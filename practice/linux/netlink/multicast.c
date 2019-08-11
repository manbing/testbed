#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <net/netlink.h>
#include <net/sock.h>
#include <linux/workqueue.h>

#define MY_GRP (1 << 1)
#define MY_CHANNEL (31)

struct sock *nl_sk = NULL;
static void send_msg(struct work_struct *work);
//static DECLARE_TASKLET(tl, send_msg, 0);
struct work_struct work;
static atomic_t run = ATOMIC_INIT(1);

static void send_msg(struct work_struct *work)
{
    struct sk_buff *skb = NULL;
    char msg [64] = "Message from Linux kernel";
    int size = 0, ret = 0;
    struct nlmsghdr *nlh = NULL;

    while (atomic_read(&run)) {
        msleep(3000);
        size = sizeof(msg);
        skb = nlmsg_new(size, GFP_KERNEL);

        if (!skb) {
            printk(KERN_ERR "%s: Cannot create netlink message\n", __func__);
            return;
        }

        nlh = nlmsg_put(skb, 0, 1, NLMSG_DONE, size, 0);
        memcpy(NLMSG_DATA(nlh), &msg, sizeof(msg));

        ret = nlmsg_multicast(nl_sk, skb, 0, MY_GRP, 0);

        if (ret < 0 ) {
            printk("%s: Send message failure\n", __func__);
            continue;
        }

        printk("%s: Send message successfully\n", __func__);
    }
    return;
}

static int __init init(void)
{
    struct netlink_kernel_cfg cfg = {
        .groups = MY_GRP,
    };

    nl_sk = netlink_kernel_create(&init_net, MY_CHANNEL, &cfg);

    if (!nl_sk) {
        printk(KERN_ERR "%s: Cannot create netlink socket\n", __func__);
        return -ENOMEM;
    }

    /* Tasklet */
    //tasklet_schedule(&tl);

    /* Workqueue */
    INIT_WORK(&work, send_msg);
    schedule_work(&work);
    return 0;
}

static void __exit exit(void)
{
    netlink_kernel_release(nl_sk);
    
    /* Tasklet */
    //tasklet_disable(&tl);
    //tasklet_kill(&tl);
    
    /* Workqueue */
    atomic_set(&run, 0);
    cancel_work_sync(&work);
}

module_init(init);
module_exit(exit);
MODULE_LICENSE("GPL");
