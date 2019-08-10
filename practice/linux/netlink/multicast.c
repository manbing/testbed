#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <net/netlink.h>
#include <net/sock.h>

#define MY_GRP (1 << 1)
#define MY_CHANNEL (31)

struct sock *nl_sk = NULL;
//static void tl_f(unsigned long val);
static void send_msg(unsigned long val);
static DECLARE_TASKLET(tl, send_msg, 0);

static void send_msg(unsigned long val)
{
    struct sk_buff *skb = NULL;
    char msg [64] = "Message from Linux kernel";
    int size = 0, ret = 0;
    struct nlmsghdr *nlh = NULL;

    while (1) {
        msleep(3000);
        size = sizeof(msg);
        skb = nlmsg_new(size, GFP_KERNEL);

        if (!skb) {
            printk(KERN_ERR "%s: Cannot create netlink message\n", __func__);
            return -ENOMEM;
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
    return 0;
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

    tasklet_schedule(&tl);
    return 0;
}

static void __exit exit(void)
{
    netlink_kernel_release(nl_sk);
    tasklet_disable(&tl);
    tasklet_kill(&tl);
}

module_init(init);
module_exit(exit);
MODULE_LICENSE("GPL");
