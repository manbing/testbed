#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/igmp.h>
#include <linux/inetdevice.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/netfilter_bridge.h>
#include <linux/if_bridge.h>
#include <linux/export.h>

/*
 *      Display an IP address in readable format.
 */

#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]
#define NIPQUAD_FMT "%u.%u.%u.%u"

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Manbing <manbing3@gmail.com>");
MODULE_DESCRIPTION("SKB tools");

static unsigned int examine_packet(unsigned int hook, struct sk_buff *skb,
                                     const struct net_device *in,
                                     const struct net_device *out,
                                     int (*okfn)(struct sk_buff *));

struct nf_hook_ops test_forward_hook = {
                .hook = examine_packet,
                .pf = NFPROTO_IPV4,
                .hooknum = NF_INET_LOCAL_IN,
                .priority = NF_BR_PRI_FIRST,
};

static unsigned int examine_packet(unsigned int hook, struct sk_buff *skb,
				     const struct net_device *in,
				     const struct net_device *out,
				     int (*okfn)(struct sk_buff *))
{
	const u8 *mac_ptr = NULL;
	struct net_bridge *br;
	struct net_bridge_port *p = NULL;

	if (!eth_hdr(skb)) {
		printk("eth_hdr(skb) is null\n");
		goto out;
	}

	printk("skb->dev->name = [%s]\n", skb->dev->name);
	mac_ptr = (const u8 *)eth_hdr(skb)->h_source;
	printk("source mac = %x:%x:%x:%x:%x:%x\n", mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);
	mac_ptr = (const u8 *)eth_hdr(skb)->h_dest;
	printk("dest mac = %x:%x:%x:%x:%x:%x\n", mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);

	if (skb->protocol == htons(ETH_P_IP)){
		const struct iphdr *iph = ip_hdr(skb);
		printk("It is IP packet\n");
		
		printk("iph->saddr = [%pI4], iph->daddr = [%pI4]\n", &iph->saddr, &iph->daddr);

		if (ip_hdr(skb)->protocol == IPPROTO_TCP) {
			printk("It is TCP packet\n");
			
			const struct tcphdr *tcph = tcp_hdr(skb);
			//printk("tcp->source = [%x], tcp->dest = [%x]\n", tcp_hdr(skb)->source, tcp_hdr(skb)->dest);
			printk("tcp->source = [%u], tcp->dest = [%u]\n", ntohs(tcp_hdr(skb)->source), ntohs(tcp_hdr(skb)->dest));
		}
		else if (ip_hdr(skb)->protocol == IPPROTO_UDP) {
			printk("It is UDP packet\n");
			
			const struct udphdr *udph = udp_hdr(skb);
			//printk("udp->source = [%x], udp->dest = [%x]\n", udp_hdr(skb)->source, udp_hdr(skb)->dest);
			printk("udp->source = [%u], udp->dest = [%u]\n", ntohs(udp_hdr(skb)->source), ntohs(udp_hdr(skb)->dest));
		}
		else if (IPPROTO_ICMP == ip_hdr(skb)->protocol) {
			printk("It is ICMP packet\n");
		}
		else if (IPPROTO_IGMP == ip_hdr(skb)->protocol) {
			printk("it is IGMP packet\n");

			struct igmphdr *ih = igmp_hdr(skb);
			struct igmpv3_query *ih3;
			//__be32 group;
			struct br_ip br_group;

			//group = ih->group;
			br_group.u.ip4 = (__be32)ih->group;
			br_group.proto = htons(ETH_P_IP);

			if (skb->len != sizeof(*ih)) {
				ih3 = igmpv3_query_hdr(skb);

			}

			printk("group=" NIPQUAD_FMT"\n", NIPQUAD(br_group.u.ip4));
		}
	}
	else if (skb->protocol == htons(ETH_P_ARP)){
		printk("It is ARP packet\n");
	}
	
out:
	return NF_ACCEPT;
}

static int __init init(void)
{
        nf_register_hook(&test_forward_hook);
        printk("skb.ko: register hook!\n");
}

static void __exit exit(void)
{
	nf_unregister_hook(&test_forward_hook);
        printk("skb.ko: unregister hook!\n");
}

module_init(init)
module_exit(exit)
MODULE_LICENSE("GPL");
