#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kconfig.h>
#include <linux/of.h>
#include <linux/etherdevice.h>
#include <linux/smsc911x.h>
#include <linux/regulator/consumer.h>

//#include "smsc911x.h"

#define SMSC_CHIPNAME           "smsc911x"
#define SMSC911X_NUM_SUPPLIES 2

static int debug = 16;

module_param(debug, int, 0);
MODULE_PARM_DESC(debug, "Debug level (0=none,...,16=all)");

struct smsc911x_data {
	void __iomem *ioaddr;

	unsigned int idrev;

	/* used to decide which workarounds apply */
	unsigned int generation;

	/* device configuration (copied from platform_data during probe) */
	struct smsc911x_platform_config config;

	/* This needs to be acquired before calling any of below:
	 * smsc911x_mac_read(), smsc911x_mac_write()
	 */
	spinlock_t mac_lock;

	/* spinlock to ensure register accesses are serialised */
	spinlock_t dev_lock;

	struct mii_bus *mii_bus;
	unsigned int using_extphy;
	int last_duplex;
	int last_carrier;

	u32 msg_enable;
	unsigned int gpio_setting;
	unsigned int gpio_orig_setting;
	struct net_device *dev;
	struct napi_struct napi;

	unsigned int software_irq_signal;

#ifdef USE_PHY_WORK_AROUND
#define MIN_PACKET_SIZE (64)
	char loopback_tx_pkt[MIN_PACKET_SIZE];
	char loopback_rx_pkt[MIN_PACKET_SIZE];
	unsigned int resetcount;
#endif

	/* Members for Multicast filter workaround */
	unsigned int multicast_update_pending;
	unsigned int set_bits_mask;
	unsigned int clear_bits_mask;
	unsigned int hashhi;
	unsigned int hashlo;

	/* register access functions */
	const struct smsc911x_ops *ops;

	/* regulators */
	struct regulator_bulk_data supplies[SMSC911X_NUM_SUPPLIES];

	/* Reset GPIO */
	struct gpio_desc *reset_gpiod;

	/* clock */
	struct clk *clk;
};


/*
 * Request resources, currently just regulators.
 *
 * The SMSC911x has two power pins: vddvario and vdd33a, in designs where
 * these are not always-on we need to request regulators to be turned on
 * before we can try to access the device registers.
 */
static int smsc911x_request_resources(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct smsc911x_data *pdata = netdev_priv(ndev);
	int ret = 0;

	/* Request regulators */
	pdata->supplies[0].supply = "vdd33a";
	pdata->supplies[1].supply = "vddvario";
	ret = regulator_bulk_get(&pdev->dev,
			ARRAY_SIZE(pdata->supplies),
			pdata->supplies);
	if (ret) {
		/*
		 * Retry on deferrals, else just report the error
		 * and try to continue.
		 */
		if (ret == -EPROBE_DEFER)
			return ret;
		netdev_err(ndev, "couldn't get regulators %d\n",
				ret);
	}

	/* Request optional RESET GPIO */
	pdata->reset_gpiod = devm_gpiod_get_optional(&pdev->dev,
						     "reset",
						     GPIOD_OUT_LOW);

	/* Request clock */
	pdata->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(pdata->clk))
		dev_dbg(&pdev->dev, "couldn't get clock %li\n",
			PTR_ERR(pdata->clk));

	return ret;
}

/*
 * enable regulator and clock resources.
 */
static int smsc911x_enable_resources(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct smsc911x_data *pdata = netdev_priv(ndev);
	int ret = 0;

	ret = regulator_bulk_enable(ARRAY_SIZE(pdata->supplies),
			pdata->supplies);
	if (ret)
		netdev_err(ndev, "failed to enable regulators %d\n",
				ret);

	if (!IS_ERR(pdata->clk)) {
		ret = clk_prepare_enable(pdata->clk);
		if (ret < 0)
			netdev_err(ndev, "failed to enable clock %d\n", ret);
	}

	return ret;
}

/*
 * disable resources, currently just regulators.
 */
static int smsc911x_disable_resources(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct smsc911x_data *pdata = netdev_priv(ndev);
	int ret = 0;

	ret = regulator_bulk_disable(ARRAY_SIZE(pdata->supplies),
			pdata->supplies);

	if (!IS_ERR(pdata->clk))
		clk_disable_unprepare(pdata->clk);

	return ret;
}

/*
 * Free resources, currently just regulators.
 *
 */
static void smsc911x_free_resources(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct smsc911x_data *pdata = netdev_priv(ndev);

	/* Free regulators */
	regulator_bulk_free(ARRAY_SIZE(pdata->supplies),
			pdata->supplies);

	/* Free clock */
	if (!IS_ERR(pdata->clk)) {
		clk_put(pdata->clk);
		pdata->clk = NULL;
	}
}

static int smsc911x_read_device_tree(struct smsc911x_platform_config *config,
				 struct device *dev)
{
	u32 width = 0;
	int err;

	err = device_property_read_u32(dev, "reg-io-width", &width);

	if (err == -ENXIO)
		return error;

	if (!err && width == 4)
		config->flags |= SMSC911X_USE_32BIT;
	else
		config->flags |= SMSC911X_USE_16BIT;

	return 0;
}


static inline u32 __smsc911x_reg_read(struct smsc911x_data *pdata, u32 reg)
{
	if (pdata->config.flags & SMSC911X_USE_32BIT)
		return readl(pdata->ioaddr + reg);

	if (pdata->config.flags & SMSC911X_USE_16BIT)
		return ((readw(pdata->ioaddr + reg) & 0xFFFF) |
			((readw(pdata->ioaddr + reg + 2) & 0xFFFF) << 16));

	BUG();
	return 0;
}

static inline void __smsc911x_reg_write(struct smsc911x_data *pdata, u32 reg,
					u32 val)
{
	if (pdata->config.flags & SMSC911X_USE_32BIT) {
		writel(val, pdata->ioaddr + reg);
		return;
	}

	if (pdata->config.flags & SMSC911X_USE_16BIT) {
		writew(val & 0xFFFF, pdata->ioaddr + reg);
		writew((val >> 16) & 0xFFFF, pdata->ioaddr + reg + 2);
		return;
	}

	BUG();
}

static inline void
smsc911x_rx_readfifo(struct smsc911x_data *pdata, unsigned int *buf,
		     unsigned int wordcount)
{
	unsigned long flags;

	spin_lock_irqsave(&pdata->dev_lock, flags);

	if (pdata->config.flags & SMSC911X_SWAP_FIFO) {
		while (wordcount--)
			*buf++ = swab32(__smsc911x_reg_read(pdata,
							    RX_DATA_FIFO));
		goto out;
	}

	if (pdata->config.flags & SMSC911X_USE_32BIT) {
		ioread32_rep(pdata->ioaddr + RX_DATA_FIFO, buf, wordcount);
		goto out;
	}

	if (pdata->config.flags & SMSC911X_USE_16BIT) {
		while (wordcount--)
			*buf++ = __smsc911x_reg_read(pdata, RX_DATA_FIFO);
		goto out;
	}

	BUG();
out:
	spin_unlock_irqrestore(&pdata->dev_lock, flags);
}

/* Writes a packet to the TX_DATA_FIFO */
static inline void
smsc911x_tx_writefifo(struct smsc911x_data *pdata, unsigned int *buf,
		      unsigned int wordcount)
{
	unsigned long flags;

	spin_lock_irqsave(&pdata->dev_lock, flags);

	if (pdata->config.flags & SMSC911X_SWAP_FIFO) {
		while (wordcount--)
			__smsc911x_reg_write(pdata, TX_DATA_FIFO,
					     swab32(*buf++));
		goto out;
	}

	if (pdata->config.flags & SMSC911X_USE_32BIT) {
		iowrite32_rep(pdata->ioaddr + TX_DATA_FIFO, buf, wordcount);
		goto out;
	}

	if (pdata->config.flags & SMSC911X_USE_16BIT) {
		while (wordcount--)
			__smsc911x_reg_write(pdata, TX_DATA_FIFO, *buf++);
		goto out;
	}

	BUG();
out:
	spin_unlock_irqrestore(&pdata->dev_lock, flags);
}

/* standard register acces */
static const struct smsc911x_ops standard_smsc911x_ops = {
	.reg_read = __smsc911x_reg_read,
	.reg_write = __smsc911x_reg_write,
	.rx_readfifo = smsc911x_rx_readfifo,
	.tx_writefifo = smsc911x_tx_writefifo,
};

/* Initializing private device structures, only called from probe */
static int smsc911x_init(struct net_device *dev)
{
	struct smsc911x_data *pdata = netdev_priv(dev);
	unsigned int byte_test, mask;
	unsigned int to = 100;

	SMSC_TRACE(pdata, probe, "Driver Parameters:");
	SMSC_TRACE(pdata, probe, "LAN base: 0x%08lX",
		   (unsigned long)pdata->ioaddr);
	SMSC_TRACE(pdata, probe, "IRQ: %d", dev->irq);
	SMSC_TRACE(pdata, probe, "PHY will be autodetected.");

	spin_lock_init(&pdata->dev_lock);
	spin_lock_init(&pdata->mac_lock);

	if (pdata->ioaddr == NULL) {
		SMSC_WARN(pdata, probe, "pdata->ioaddr: 0x00000000");
		return -ENODEV;
	}

	/*
	 * poll the READY bit in PMT_CTRL. Any other access to the device is
	 * forbidden while this bit isn't set. Try for 100ms
	 *
	 * Note that this test is done before the WORD_SWAP register is
	 * programmed. So in some configurations the READY bit is at 16 before
	 * WORD_SWAP is written to. This issue is worked around by waiting
	 * until either bit 0 or bit 16 gets set in PMT_CTRL.
	 *
	 * SMSC has confirmed that checking bit 16 (marked as reserved in
	 * the datasheet) is fine since these bits "will either never be set
	 * or can only go high after READY does (so also indicate the device
	 * is ready)".
	 */

	mask = PMT_CTRL_READY_ | swahw32(PMT_CTRL_READY_);
	while (!(smsc911x_reg_read(pdata, PMT_CTRL) & mask) && --to)
		udelay(1000);

	if (to == 0) {
		netdev_err(dev, "Device not READY in 100ms aborting\n");
		return -ENODEV;
	}

	/* Check byte ordering */
	byte_test = smsc911x_reg_read(pdata, BYTE_TEST);
	SMSC_TRACE(pdata, probe, "BYTE_TEST: 0x%08X", byte_test);
	if (byte_test == 0x43218765) {
		SMSC_TRACE(pdata, probe, "BYTE_TEST looks swapped, "
			   "applying WORD_SWAP");
		smsc911x_reg_write(pdata, WORD_SWAP, 0xffffffff);

		/* 1 dummy read of BYTE_TEST is needed after a write to
		 * WORD_SWAP before its contents are valid */
		byte_test = smsc911x_reg_read(pdata, BYTE_TEST);

		byte_test = smsc911x_reg_read(pdata, BYTE_TEST);
	}

	if (byte_test != 0x87654321) {
		SMSC_WARN(pdata, drv, "BYTE_TEST: 0x%08X", byte_test);
		if (((byte_test >> 16) & 0xFFFF) == (byte_test & 0xFFFF)) {
			SMSC_WARN(pdata, probe,
				  "top 16 bits equal to bottom 16 bits");
			SMSC_TRACE(pdata, probe,
				   "This may mean the chip is set "
				   "for 32 bit while the bus is reading 16 bit");
		}
		return -ENODEV;
	}

	/* Default generation to zero (all workarounds apply) */
	pdata->generation = 0;

	pdata->idrev = smsc911x_reg_read(pdata, ID_REV);
	switch (pdata->idrev & 0xFFFF0000) {
	case 0x01180000:
	case 0x01170000:
	case 0x01160000:
	case 0x01150000:
	case 0x218A0000:
		/* LAN911[5678] family */
		pdata->generation = pdata->idrev & 0x0000FFFF;
		break;

	case 0x118A0000:
	case 0x117A0000:
	case 0x116A0000:
	case 0x115A0000:
		/* LAN921[5678] family */
		pdata->generation = 3;
		break;

	case 0x92100000:
	case 0x92110000:
	case 0x92200000:
	case 0x92210000:
		/* LAN9210/LAN9211/LAN9220/LAN9221 */
		pdata->generation = 4;
		break;

	default:
		SMSC_WARN(pdata, probe, "LAN911x not identified, idrev: 0x%08X",
			  pdata->idrev);
		return -ENODEV;
	}

	SMSC_TRACE(pdata, probe,
		   "LAN911x identified, idrev: 0x%08X, generation: %d",
		   pdata->idrev, pdata->generation);

	if (pdata->generation == 0)
		SMSC_WARN(pdata, probe,
			  "This driver is not intended for this chip revision");

	/* workaround for platforms without an eeprom, where the mac address
	 * is stored elsewhere and set by the bootloader.  This saves the
	 * mac address before resetting the device */
	if (pdata->config.flags & SMSC911X_SAVE_MAC_ADDRESS) {
		spin_lock_irq(&pdata->mac_lock);
		smsc911x_read_mac_address(dev);
		spin_unlock_irq(&pdata->mac_lock);
	}

	/* Reset the LAN911x */
	if (smsc911x_phy_reset(pdata) || smsc911x_soft_reset(pdata))
		return -ENODEV;

	dev->flags |= IFF_MULTICAST;
	netif_napi_add(dev, &pdata->napi, smsc911x_poll, SMSC_NAPI_WEIGHT);
	dev->netdev_ops = &smsc911x_netdev_ops;
	dev->ethtool_ops = &smsc911x_ethtool_ops;

	return 0;
}

static int smsc911x_probe(struct platform_device *pdev)
{
	printk("manbing [%s][%d]\n", __func__, __LINE__);
	struct resource *res = NULL;
	int irq = 0, retval = 0, ph_mem_size = 0;
	struct net_device *dev = NULL;
	struct smsc911x_data *pdata = NULL;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!res) {
		pr_warning("Could not allocate resource\n");
		goto out;
	}
	
	ph_mem_size = resource_size(res);
	
	if (!request_mem_region(res->start, ph_mem_size, SMSC_CHIPNAME)) {
		retval = -EBUSY;
		goto out;
	}

	irq = platform_get_irq(pdev,0);
	if (irq == -EPROBE_DEFER) {
		retval = -EPROBE_DEFER;
	}
	else if (irq <= 0) {
		pr_warning("Could not allocate irq resource\n");
		retval = -ENODEV;
		goto out;
	}

	dev = alloc_etherdev(sizeof(struct smsc911x_data));
	if (!dev) {
		retval = -ENODEV;
		goto out_release_mem;
	}

	SET_NETDEV_DEV(dev, &pdev->dev);
	
	pdata = netdev_priv(dev);
	dev->irq = irq;
	pdata->ioaddr = ioremap_noncache(res->start, ph_mem_size);
	pdata->dev = dev;
	platform_set_drvdata(pdev, dev);

	retval = smsc911x_request_resources(pdev);
	if (retval < 0)
		goto out_request_resources_fail;

	retval = smsc911x_enable_resources(pdev);
	if (retval < 0)
		goto out_enable_resources_fail;

	if (pdata->ioaddr == NULL) {
		pr_warning("Error smsc911x base address invalid\n");
		retval = -ENOMEM;
		goto out_disable_resources;
	}

	retval = smsc911x_read_device_tree(&pdata->config, &pdev->dev);
	pdata->ops = &standard_smsc911x_ops;

	pm_runtime_enable(&pdev->dev);
	pm_runtime_get_sync(&pdev->dev);

	retval = smsc911x_init(dev);
	if (retval < 0)
		goto out_disable_resources;

	return 0;

out_disable_resources:
	smsc911x_disable_resources(pdev);

out_enable_resources_fail:
	smsc911x_free_resources(pdev);

out_request_resources_fail:
	iounmap(pdata->ioaddr);
	free_netdev(dev);
	
out_release_mem:
	release_mem_region(res->start, ph_mem_size);

out:
	return retval;
}

static int smsc911x_remove(struct platform_device *pdev)
{
	int retval = 0;

	printk("manbing [%s][%d]\n", __func__, __LINE__);
	return retval;
}


#ifdef CONFIG_OF
static const struct of_device_id smsc911x_dt_ids[] = {
	{ .compatible = "smsc,lan9115", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, smsc911x_dt_ids);
#endif

static struct platform_driver smsc911x_driver = {
	.probe = smsc911x_probe,
	.remove = smsc911x_remove,
	.driver = {
		.name = SMSC_CHIPNAME,
		.of_match_table = of_match_ptr(smsc911x_dt_ids),
	},
};

static int __init smsc911x_init_private_module(void)
{
	return platform_driver_register(&smsc911x_driver);
}

static void __exit smsc911x_exit_private_module(void)
{
	platform_driver_unregister(&smsc911x_driver);
}

module_init(smsc911x_init_private_module);
module_exit(smsc911x_exit_private_module);
