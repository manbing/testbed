#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>

#define MY_GRP (1 << 1)
#define MY_CHANNEL (31)

struct sockaddr_nl nl_addr;

int open_nl()
{
    int sock = socket(AF_NETLINK, SOCK_RAW, MY_CHANNEL);

    if (sock < 0) {
        return -1;
    }

    memset(&nl_addr, 0, sizeof(nl_addr));
    nl_addr.nl_family = AF_NETLINK;
    //nl_addr.nl_pid = getpid();
    nl_addr.nl_groups = MY_GRP;

    if (bind(sock, (struct sockaddr *)&nl_addr, sizeof(nl_addr)) < 0) {
        printf("%s: bind failure\n", __func__);
        return -1;
    }

    return sock;
}

int read_msg(int sock)
{
    int ret = 0, i = 0, len = 0;
    struct nlmsghdr nlh, *tmp;
    struct iovec iov;
    char buffer [1024] = {0};
    struct msghdr msg;
    char *ptr = NULL;

    iov.iov_base = (void *)&buffer;
    iov.iov_len = sizeof(buffer);

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&nl_addr;
    msg.msg_namelen = sizeof(nl_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    ret = recvmsg(sock, &msg, 0);

    if (ret < 0 ) {
        printf("%s: receive message failure\n", __func__);
        return -1;
    }

    for ((tmp = (struct nlmsghdr *)buffer); NLMSG_OK(tmp, ret); tmp = NLMSG_NEXT(tmp, ret)) {
	if (tmp->nlmsg_type == NLMSG_DONE) {
	    ptr = NLMSG_DATA(tmp);
	    printf("Received message payload: %s\n", ptr);
	    printf("\n\n");
	}
    }

    return 0;
}

int main (void)
{
    int sock = 0;
    fd_set rfds;

    sock = open_nl();

    if (sock < 0) {
        printf("Open netlink failure\n");
        return -1;
    }

    while (1) {
        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);

        select(sock + 1, &rfds, NULL, NULL, NULL);

        if (FD_ISSET(sock, &rfds)) {
            read_msg(sock);
        }

    };

    return 0;
}
