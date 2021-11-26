#if 0
/*
 * linux/Documentation/bpf/bpf_devel_QA.rst
 *
 * - BPF program may recursively include header file(s) with file scope
 *  inline assembly codes. The default target can handle this well,
 *  while ``bpf`` target may fail if bpf backend assembler does not
 *  understand these assembly codes, which is true in most cases.
 */

#include <linux/err.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netpoll.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>
#include <net/tcp.h>
#endif

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <uapi/linux/bpf.h>
#include <linux/version.h>
#include "bpf_helpers.h"

#define _(P) ({typeof(P) val = 0; bpf_probe_read(&val, sizeof(val), &P); val;})

#if 0
/*
 * linux/Documentation/bpf/bpf_devel_QA.rst
 *
 * Q: Can loops be supported in a safe way?
 * ----------------------------------------
 * A: It's not clear yet.
 *
 * BPF developers are trying to find a way to
 * support bounded loops.
 */
#define bpf_trace_printk_str(x) {	\
		char fmt[] = "%c";		\
		char fmt2[] = "\n";		\
		int i = 0;		\
		\
		while (x[i] != '/0')\
			bpf_trace_printk(fmt, sizeof(fmt), x[i++]);	\
						\
		bpf_trace_printk(fmt2, sizeof(fmt2));\
	} while (0)
#endif

#define bpf_trace_printk_str2(x) {	\
		char fmt[] = "%d";		\
		char fmt2[] = "\n";		\
		int i = 0;		\
		\
		bpf_trace_printk(fmt, sizeof(fmt), x[i++]);	\
		bpf_trace_printk(fmt, sizeof(fmt), x[i++]);	\
		bpf_trace_printk(fmt2, sizeof(fmt2));\
	} while (0)

int bpf_prog1(struct pt_regs *ctx)
{
	struct sk_buff *skb;
	struct net_device *dev;
	char devname[IFNAMSIZ];
	char fmt[] = "device name =%s\n";
	int len;

	skb = (struct sk_buff *) PT_REGS_PARM1(ctx);
	dev = _(skb->dev);
	len = _(skb->len);

/*
 * linux/Documentation/bpf/bpf_devel_QA.rst
 *
 * Tracing bpf programs can *read* arbitrary memory with bpf_probe_read()
 * and bpf_probe_read_str() helpers. Networking programs cannot read
 * arbitrary memory, since they don't have access to these helpers.
 * Programs can never read or write arbitrary memory directly.
 */
	bpf_probe_read(devname, sizeof(devname), dev->name);
	bpf_trace_printk_str2(devname);
	return 0;
}
