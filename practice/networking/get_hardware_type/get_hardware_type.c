#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
        int fd = -1, i = 0;
        struct ifreq ifr = {0};
        char *nic = NULL;
        unsigned char *mac = NULL;

        if (argc != 2) {
                printf("usage e.g., get_hardware_type eth0\n");
                return -1;
        }

        nic = argv[1];
        fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        perror("socket");
        strncpy(ifr.ifr_name, nic, IFNAMSIZ-1);
        if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1) {
                if (errno == ENODEV) {
                        printf("No such device.\n");
                        return -1;
                }    
                
                perror("ioctl");
                return -1;
        }    

        printf("ifr.ifr_hwaddr.sa_family = %d\n", ifr.ifr_hwaddr.sa_family);
        mac = (unsigned char *) ifr.ifr_hwaddr.sa_data;

        printf("Mac : %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        return 0;
}
