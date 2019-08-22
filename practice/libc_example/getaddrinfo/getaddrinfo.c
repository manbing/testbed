#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <paths.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <syslog.h>

static struct sockaddr_in remoteaddr;	/* leon for remote syslogd */
static int  remotefd 		= -1;	 /* udp socket for logging to remote host */

static void send_msg(int pri, char *msg)
{
    time_t now;
    char *timestamp;
    static char res[20] = "";
    char *ptr;
    char tmpBuf[1600];	/* leon for remote syslogd */

    /* send message to remote logger */
    if (-1 != remotefd) {
        static const int IOV_COUNT = 2;
        struct iovec iov[IOV_COUNT];
        struct iovec *v = iov;

        memset(&res, 0, sizeof(res));
        snprintf(res, sizeof(res), "<%d>", LOG_INFO);
        v->iov_base = res ;
        v->iov_len = strlen(res);
        v++;

        v->iov_base = msg;
        v->iov_len = strlen(msg);
writev_retry:
        sprintf(tmpBuf, "%s%s", res, msg);
        if (sendto(remotefd, tmpBuf, strlen(tmpBuf), 0, (struct sockaddr*)&remoteaddr, sizeof(remoteaddr)) <= 0) {
            printf("err %d: %s\n", errno, strerror(errno));
            if (errno == EINTR) goto writev_retry;
        }
    }
}

static int init_socket(const char *remote_host, const char *remote_port)
{
    int len = sizeof(remoteaddr), ret = 0;
    struct addrinfo hints;
    int port = atoi(remote_port);
    struct addrinfo *result;

    memset(&remoteaddr, 0, len);
    memset(&hints, 0, sizeof(hints));

    remotefd = socket(AF_INET, SOCK_DGRAM, 0);

    if (remotefd < 0)  {
        printf("cannot create socket\n");
        return -1;
    }

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;

    if ((ret = getaddrinfo(remote_host, remote_port, &hints, &result)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(ret));
        return -1;
    }

    remoteaddr = *(struct  sockaddr_in *)result->ai_addr;
    freeaddrinfo(result);
    return 0;
}

int main(int argc, char *argv[])
{
    init_socket(argv[1], argv[2]);

    while (1) {
        send_msg(192, "Test syslog message");
        sleep(3);
    }
    
    return 0;
}
