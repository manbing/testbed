#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define NAMESPACE "/var/run/netns/cindy"

int main(void)
{
    int fd;

    /* Check file exist */
    if ( -1 == (fd = open(NAMESPACE, O_RDONLY))) {
        printf("Name space is not existing!\n");
        exit(-1);
    }

    if (setns(fd, CLONE_NEWNET) == -1) {
        printf("Assigned name space fail!\n");
        printf("%s,%d\n", strerror(errno), errno);
        exit(-1);
    }

    sleep(300);


    return 0;
}
