#define _GNU_SOURCE
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stropts.h>
#include <linux/bpf.h>
#include <linux/version.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <errno.h>


int main(int argc, char *argv[])
{
	int bfd;
	int ret;
	int efd;
	int pfd;
	int n;
	int i;
	struct bpf_insn *insn;
	union bpf_attr attr = {};
	struct perf_event_attr pattr = {};
	unsigned char filename [64] = {};
	unsigned char buf [1024] = {};
	unsigned char log_buf [4096] = {};

	bfd = open("bpf_prog", O_RDONLY);
	if (bfd < 0)
	{
		printf("open eBPF program error: %s\n", strerror(errno));
		exit(-1);
	}
	n = read(bfd, buf, 1024);
	for (i = 0; i < n; ++i)
	{
		printf("%02x ", buf[i]);
		if (i % 8 == 0)
			printf("\n");
	}
	close(bfd);
	insn = (struct bpf_insn*)buf;
	attr.prog_type = BPF_PROG_TYPE_KPROBE;
	attr.insns = (unsigned long)insn;
	attr.insn_cnt = n / sizeof(struct bpf_insn);
	attr.license = (unsigned long)"GPL";
	attr.log_size = sizeof(log_buf);
	attr.log_buf = (unsigned long)log_buf;
	attr.log_level = 1;
	attr.kern_version = 264656;
	pfd = syscall(SYS_bpf, BPF_PROG_LOAD, &attr, sizeof(attr));
	if (pfd < 0)
	{
		printf("bpf syscall error: %s\n", strerror(errno));
		printf("log_buf = %s\n", log_buf);
		exit(-1);
	}

	pattr.type = PERF_TYPE_TRACEPOINT;
	pattr.sample_type = PERF_SAMPLE_RAW;
	pattr.sample_period = 1;
	pattr.wakeup_events = 1;
	pattr.config = 1550;
	pattr.size = sizeof(pattr);
	efd = syscall(SYS_perf_event_open, &pattr, -1, 0, -1, 0);
	if (efd < 0)
	{
		printf("perf_event_open error: %s\n", strerror(errno));
		exit(-1);
	}
	ret = ioctl(efd, PERF_EVENT_IOC_ENABLE, 0);
	if (ret < 0)
	{
		printf("PERF_EVENT_IOC_ENABLE error: %s\n", strerror(errno));
		exit(-1);
	}
	ret = ioctl(efd, PERF_EVENT_IOC_SET_BPF, pfd);
	if (ret < 0)
	{
		printf("PERF_EVENT_IOC_SET_BPF error: %s\n", strerror(errno));
		exit(-1);
	}
	while(1);
}
