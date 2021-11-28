#include <linux/err.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netpoll.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>
#include <net/tcp.h>

void show_skb(struct sk_buff *skb)
{
	printk("[%s][%s][%d] start \n", __FILE__, __func__, __LINE__);
	
	const u8 *mac_ptr = NULL;

	printk("skb->dev->name = [%s]\n", __FILE__, __func__, __LINE__, skb->dev->name);
	mac_ptr = (const u8 *)eth_hdr(skb)->h_source;
	printk("source mac = %x:%x:%x:%x:%x:%x\n", mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);
	
	if (skb->protocol == htons(ETH_P_IP)){
		const struct iphdr *iph = ip_hdr(skb);
		printk("It is IP packet\n");
		
		printk("iph->saddr = [%pI4], iph->daddr = [%pI4]\n", &iph->saddr, &iph->daddr);

		if (ip_hdr(skb)->protocol == IPPROTO_TCP){
			printk("It is TCP packet\n");
			
			const struct tcphdr *tcph = tcp_hdr(skb);
			//printk("tcp->source = [%x], tcp->dest = [%x]\n", tcp_hdr(skb)->source, tcp_hdr(skb)->dest);
			printk("tcp->source = [%u], tcp->dest = [%u]\n", ntohs(tcp_hdr(skb)->source), ntohs(tcp_hdr(skb)->dest));
		}
		else if (ip_hdr(skb)->protocol == IPPROTO_UDP){
			printk("It is UDP packet\n");
			
			const struct udphdr *udph = udp_hdr(skb);
			//printk("udp->source = [%x], udp->dest = [%x]\n", udp_hdr(skb)->source, udp_hdr(skb)->dest);
			printk("udp->source = [%u], udp->dest = [%u]\n", ntohs(udp_hdr(skb)->source), ntohs(udp_hdr(skb)->dest));
		}
	}
	else if (skb->protocol == htons(ETH_P_ARP)){
		printk("It is ARP packet\n");
	}
	
	printk("[%s][%s][%d] end \n", __FILE__, __func__, __LINE__);
}
EXPORT_SYMBOL(show_skb);
