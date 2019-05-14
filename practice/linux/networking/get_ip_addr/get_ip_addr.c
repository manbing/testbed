#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <net/ethernet.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>

/*
 *  that ioctl is legacy and won't return IPv6. Each platform has a different way of getting the IPv6 ones:
 *  @ Linux, use NETLINK if you're crazy, use getifaddrs if you have a vaguely recent glibc, otherwise read /proc/net/if_inet6 (eg on Android).
 *  @ Darwin or FreeBSD: use getifaddrs.
 *  @ Solaris, use SIOCGLIFCONF.
 *  @ AIX, use SIOCGIFCONF which actually returns IPv4 and IPv6 addresses (because they have an sa_len field in struct sockaddr they can actually support that).
 */

#define IPV6_ADDR_GLOBAL        0x0000U
#define IPV6_ADDR_LOOPBACK      0x0010U
#define IPV6_ADDR_LINKLOCAL     0x0020U
#define IPV6_ADDR_SITELOCAL     0x0040U
#define IPV6_ADDR_COMPATv4      0x0080U

#define RED_BOLD "\x1b[;31;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define GRN_BOLD "\x1b[;32;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define RESET "\x1b[0;m"

static void _parse_inet6(const char *ifname) {
    FILE *f;
    int ret, scope, prefix;
    unsigned char ipv6[16];
    char dname[IFNAMSIZ];
    char address[INET6_ADDRSTRLEN];
    char *scopestr;

    printf(RED_BOLD "\nGet IPv6 address, Read /proc/net/if_inet6.....\n" RESET);

    f = fopen("/proc/net/if_inet6", "r");
    if (f == NULL) {
        return;
    }

    while (19 == fscanf(f,
                " %2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx %*x %x %x %*x %s",
                &ipv6[0],
                &ipv6[1],
                &ipv6[2],
                &ipv6[3],
                &ipv6[4],
                &ipv6[5],
                &ipv6[6],
                &ipv6[7],
                &ipv6[8],
                &ipv6[9],
                &ipv6[10],
                &ipv6[11],
                &ipv6[12],
                &ipv6[13],
                &ipv6[14],
                &ipv6[15],
                &prefix,
                &scope,
                dname)) {

        if (strcmp(ifname, dname) != 0) {
            continue;
        }

        if (inet_ntop(AF_INET6, ipv6, address, sizeof(address)) == NULL) {
            continue;
        }

        switch (scope) {
            case IPV6_ADDR_GLOBAL:
                scopestr = "Global";
                break;
            case IPV6_ADDR_LINKLOCAL:
                scopestr = "Link";
                break;
            case IPV6_ADDR_SITELOCAL:
                scopestr = "Site";
                break;
            case IPV6_ADDR_COMPATv4:
                scopestr = "Compat";
                break;
            case IPV6_ADDR_LOOPBACK:
                scopestr = "Host";
                break;
            default:
                scopestr = "Unknown";
        }

        printf("IPv6 address: %s, prefix: %d, scope: %s\n", address, prefix, scopestr);
    }

    fclose(f);
}

static void _get_ip_addr(const char *ifname)
{
    int sock;
    struct ifreq ifr;
    struct sockaddr_in *ipaddr;
    char address[INET_ADDRSTRLEN];
    size_t ifnamelen;

    printf(RED_BOLD "\nGet IP address, Using ioctl.....\n" RESET);

    /* copy ifname to ifr object */
    ifnamelen = strlen(ifname);
    if (ifnamelen >= sizeof(ifr.ifr_name)) {
        return ;
    }
    memcpy(ifr.ifr_name, ifname, ifnamelen);
    ifr.ifr_name[ifnamelen] = '\0';

    /* open socket */
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        return;
    }

    /* process mac */
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) != -1) {
        printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                (unsigned char)ifr.ifr_hwaddr.sa_data[0],
                (unsigned char)ifr.ifr_hwaddr.sa_data[1],
                (unsigned char)ifr.ifr_hwaddr.sa_data[2],
                (unsigned char)ifr.ifr_hwaddr.sa_data[3],
                (unsigned char)ifr.ifr_hwaddr.sa_data[4],
                (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
    }

    /* process mtu */
    if (ioctl(sock, SIOCGIFMTU, &ifr) != -1) {
        printf("MTU: %d\n", ifr.ifr_mtu);
    }

    /* die if cannot get address */
    if (ioctl(sock, SIOCGIFADDR, &ifr) == -1) {
        close(sock);
        return;
    }

    /* process ip */
    ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;
    if (inet_ntop(AF_INET, &ipaddr->sin_addr, address, sizeof(address)) != NULL) {
        printf("Ip address: %s\n", address);
    }

    /* try to get broadcast */
    if (ioctl(sock, SIOCGIFBRDADDR, &ifr) != -1) {
        ipaddr = (struct sockaddr_in *)&ifr.ifr_broadaddr;
        if (inet_ntop(AF_INET, &ipaddr->sin_addr, address, sizeof(address)) != NULL) {
            printf("Broadcast: %s\n", address);
        }
    }

    /* try to get mask */
    if (ioctl(sock, SIOCGIFNETMASK, &ifr) != -1) {
        ipaddr = (struct sockaddr_in *)&ifr.ifr_netmask;
        if (inet_ntop(AF_INET, &ipaddr->sin_addr, address, sizeof(address)) != NULL) {
            printf("Netmask: %s\n", address);
        }
    }

    close(sock);
}

static int _get_ipv6_addr(const char *ifname)
{
    struct ifaddrs *stIfaddrs = (struct ifaddrs *)NULL;
    struct ifaddrs *pNext = (struct ifaddrs *)NULL;
    int nRet = -1, i = -1;
    struct sockaddr_in *pSockAddr_In4 = NULL;
    struct sockaddr_in6 *pSockAddr_In6 = NULL;
    char strIPv4Addr[INET_ADDRSTRLEN];
    char ip_str [128] = {0};

    printf(RED_BOLD "\nGet All IP address, Using getifaddrs.....\n" RESET);

    nRet = getifaddrs(&stIfaddrs);
    if (nRet == -1) {
        herror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (pNext = stIfaddrs; pNext != (struct ifaddrs *)NULL; \
            pNext = pNext->ifa_next) {
        if (pNext->ifa_addr->sa_family == AF_INET) {
            printf("IPv4 Interface (AF_INET, %d): \n", \
                    pNext->ifa_addr->sa_family);
            // Interface Name
            printf("pNext->ifa_name: %s \n", pNext->ifa_name);
            printf("pNext->ifa_flags: %d \n", pNext->ifa_flags);
            // Interface Address
            printf("IPv4 Address: \n");
            pSockAddr_In4 = (struct sockaddr_in *)pNext->ifa_addr;
            printf("pSockAddr_In4->sin_family: %d \n", \
                    pSockAddr_In4->sin_family);
            printf("pSockAddr_In4->sin_port: %d \n", \
                    ntohs(pSockAddr_In4->sin_port));
            memset(strIPv4Addr, '\0', \
                    sizeof(char) * INET_ADDRSTRLEN);
            inet_ntop(AF_INET, (void *)&pSockAddr_In4->sin_addr, \
                    strIPv4Addr, sizeof(char) * INET_ADDRSTRLEN);
            printf("pSockAddr_In4->sin_addr: %s \n", strIPv4Addr);
            // Interface Netmask
            printf("IPv4 Address Netmask: \n");
            pSockAddr_In4 = \
                            (struct sockaddr_in *)pNext->ifa_netmask;
            printf("pSockAddr_In4->sin_family: %d \n", \
                    pSockAddr_In4->sin_family);
            printf("pSockAddr_In4->sin_port: %d \n", \
                    ntohs(pSockAddr_In4->sin_port));
            memset(strIPv4Addr, '\0', \
                    sizeof(char) * INET_ADDRSTRLEN);
            inet_ntop(AF_INET, (void *)&pSockAddr_In4->sin_addr, \
                    strIPv4Addr, sizeof(char) * INET_ADDRSTRLEN);
            printf("pSockAddr_In->sin_addr: %s \n", strIPv4Addr);
            // Interface Broadcast
            printf("IPv4 Broadcast Address: \n");
            pSockAddr_In4 = \
                            (struct sockaddr_in *)pNext->ifa_ifu.ifu_broadaddr;
            /*pSockAddr_In4 = \
              (struct sockaddr_in *)pNext->ifa_ifu.ifu_dstaddr;
             */printf("pSockAddr_In4->sin_family: %d \n", \
                     pSockAddr_In4->sin_family);
             printf("pSockAddr_In4->sin_port: %d \n", \
                     ntohs(pSockAddr_In4->sin_port));
             memset(strIPv4Addr, '\0', \
                     sizeof(char) * INET_ADDRSTRLEN);
             inet_ntop(AF_INET, (void *)&pSockAddr_In4->sin_addr, \
                     strIPv4Addr, sizeof(char) * INET_ADDRSTRLEN);
             printf("pSockAddr_In4->sin_addr: %s \n", strIPv4Addr);
             // Address-specific data (may be unused). 
             nRet = sizeof(pNext->ifa_data);
             printf("pNext->ifa_data:");
             for (i = 0; i < nRet; i++)
                 printf(" %02X", (pNext->ifa_data) + i);
             printf(" (%d) \n", nRet);
        }
        else if (pNext->ifa_addr->sa_family == AF_INET6) {
            pSockAddr_In6 = (struct sockaddr_in6 *)pNext->ifa_addr;
            inet_ntop(AF_INET6, (void *)&pSockAddr_In6->sin6_addr, ip_str, sizeof(ip_str));
            printf("pSockAddr_In6->sin6_addr: %s \n", ip_str);
        }
        else if (pNext->ifa_addr->sa_family == AF_PACKET) { }
        else { }
        printf("\n");
    }
    freeifaddrs((void *)stIfaddrs);

    return 0;
}

int main(int argc, char *argv[])
{
    int fd = -1, i = 0;
    struct ifreq ifr = {0};
    char *nic = NULL;
    unsigned char *mac = NULL;

    if (argc != 2) {
        printf("usage e.g., get_ip_addr eth0\n");
        return -1;
    }

    nic = argv[1];

    _get_ip_addr(nic);
    _get_ipv6_addr(nic);
    _parse_inet6(nic);

    return 0;
}
