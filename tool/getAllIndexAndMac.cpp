#include <string>           
#include <cstring>              // For memcpy() and other useful nifty fct
#include <net/if.h>             // struct ifreq
#include <linux/if_ether.h>     // ETH_P_IP = 0x0800, ETH_P_IPV6 = 0x86DD
#include <arpa/inet.h>          // For htons
#include <unistd.h>             // close()
#include <sys/ioctl.h>          // macro ioctl is defined
#include <linux/if_packet.h>    // struct sockaddr_ll (see man 7 packet)

using namespace std;

char *allocate_strmem (int);
uint8_t *allocate_ustrmem (int);

int main(int agrc, char *argv[]) {
    int sd;
    char *interface, *target;
    uint8_t *src_mac, *dst_mac;
    struct ifreq ifr;
    struct sockaddr_ll device;

    // Allocate some dynamic mem for our c-string.
    src_mac = new uint8_t[6];
    dst_mac = new uint8_t[6];
    interface = new char[40];
    target = new char[40];
    
    // Submit request for a socket descriptor to look up interface.
    if ((sd = socket (AF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
        perror ("socket() failed to get socket descriptor for using ioctl() ");
        exit (EXIT_FAILURE);
    }

    struct if_nameindex *if_ni, *if_i;

    if_ni = if_nameindex();
    if( if_ni == NULL ) {
        perror("if_nameindex");
        exit(EXIT_FAILURE);
    }

    for( if_i = if_ni; !(if_i->if_index == 0 && if_i->if_name == NULL); ++if_i ) {
        // Interface to send packet through.
        strcpy(interface, if_i->if_name);
        printf("'%s'\n", interface);

        // Use ioctl() to look up interface name and get its MAC address.
        memset (&ifr, 0, sizeof (ifr));
        memcpy (ifr.ifr_name, interface, sizeof (ifr.ifr_name));
        if (ioctl (sd, SIOCGIFHWADDR, &ifr) < 0) {
            perror ("ioctl() failed to get source MAC address ");
            return (EXIT_FAILURE);
        }
        
        // Copy source MAC address.
        memcpy (src_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));

        // Report source MAC address to stdout.
        printf ("MAC address for interface '%s' is: '", interface);
        for (int i=0; i<6; ++i) {
            printf ("%02x:", src_mac[i]);
        }
        printf("'\n");

        // Find interface index from interface name and store index in
        // struct sockaddr_ll device, which will be used as an argument of sendto().
        memset (&device, 0, sizeof (device));
        if ((device.sll_ifindex = if_nametoindex (interface)) == 0) {
            perror ("if_nametoindex() failed to obtain interface index ");
            exit (EXIT_FAILURE);
        }
        printf ("Index for interface %s is %i\n", interface, device.sll_ifindex);
    }


    close(sd);

    // Free our mem
    delete src_mac;
    delete dst_mac;
    delete interface;
    delete target;
    if_freenameindex(if_ni);
    

    return 0;
}