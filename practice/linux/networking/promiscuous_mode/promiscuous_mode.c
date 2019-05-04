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
#include <unistd.h>

static int open_packet_socket(int ifindex, uint16_t eth_type)
{
        int                 s;
        struct sockaddr_ll  socket_address;

        s = socket(AF_PACKET, SOCK_RAW, htons(eth_type));
        if (-1 == s)
        {
                return -1;
        }

        memset(&socket_address, 0, sizeof(socket_address));
        socket_address.sll_family   = AF_PACKET;
        socket_address.sll_ifindex  = ifindex;
        socket_address.sll_protocol = htons(eth_type);

        if (-1 == bind(s, (struct sockaddr*)&socket_address, sizeof(socket_address)))
        {
                close(s);
                return -1;
        }

        return s;
}

static int get_if_index(const char *if_name)
{
        int                 s;
        struct ifreq        ifr;

        s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        if (-1 == s)
        {
                printf("[PLATFORM] socket('%s') returned with errno=%d (%s) while opening a RAW socket\n",
                                if_name, errno, strerror(errno));
                return -1;
        }

        strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
        if (ioctl(s, SIOCGIFINDEX, &ifr) == -1)
        {
                printf("[PLATFORM] ioctl('%s',SIOCGIFINDEX) returned with errno=%d (%s) while opening a RAW socket\n",
                                if_name, errno, strerror(errno));
                close(s);
                return -1;
        }
        close(s);
        return ifr.ifr_ifindex;
}

int main(int argc, char *argv[])
{
        int fd = -1, index, i = 0;
        char *nic = NULL, *mac = NULL;
        struct packet_mreq request;
        char mac_address [6] = {0};

        if (!argc == 2 && argc != 3) {
                printf("usage e.g., promiscuous_mode eth0 {MAC address}\n");
                return -1;
        }

        if (argc == 2) {
                nic = argv[1];
        } else if (argc == 3) {
                nic = argv[1];
                mac = argv[2];
        }

        if ((index = get_if_index(nic)) == -1) {
                printf("get_if_index fail\n");
                return -1;
        }

        if ((fd = open_packet_socket(index, ETH_P_ALL)) == -1) {
                printf("open_packet_socket fail\n");
                return -1;
        }

        memset(&request, 0, sizeof(request));
        request.mr_ifindex = index;
        request.mr_alen = 6;

        if (argc == 2) {
                /*
                 *  Method 1:
                 *  Receviced the packet of ALL MAC address, as known as "promiscuous mode"
                 */
                request.mr_type = PACKET_MR_PROMISC;
                if (-1 == setsockopt(fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &request, sizeof(request))) {
                        printf("[%s][%d] errno = %d, %s\n", __func__, __LINE__, errno, strerror(errno));
                        close(fd);
                        return -1;
                }
        } else if (argc == 3) {
                /*
                 *  Method 2:
                 *  Receviced the packet of specific MAC address
                 */
                sscanf(mac, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                                &mac_address[0],
                                &mac_address[1],
                                &mac_address[2],
                                &mac_address[3],
                                &mac_address[4],
                                &mac_address[5]);

                request.mr_type = PACKET_MR_UNICAST;
                memcpy(request.mr_address, mac_address, 6);
                if (-1 == setsockopt(fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &request, sizeof(request))) {
                        printf("[%s][%d] errno = %d, %s\n", __func__, __LINE__, errno, strerror(errno));
                        close(fd);
                        return -1;
                }
        }

        printf("Set %s as promiscuous mode successfully.\n", nic);

        for (i = 0; i < 6; i++) {
                sleep(10);
        }

        close(fd); 
        return 0;
}
