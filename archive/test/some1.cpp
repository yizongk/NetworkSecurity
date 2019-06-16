#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netpacket/packet.h> // For sockaddr_ll
#include <netinet/ether.h> // For ether_aton()
#include <arpa/inet.h> // For htons()
#include <net/ethernet.h>
#include <linux/if_ether.h>
#include <net/if.h> // For if_nametoindex()
 
#define BUF_LEN 16
 
int main ( int argc, char **argv )
{
 
int sock = 0;
int sent = 0;

void* buffer = malloc( sizeof(char) * BUF_LEN );

    struct sockaddr_ll link;
 
    memset(&link, 0, sizeof(struct sockaddr_ll));
 
    // Populate sockaddr
    link.sll_family = AF_PACKET;
    link.sll_protocol = htons(ETH_P_ARP);
    link.sll_ifindex = if_nametoindex("eth0");
    link.sll_hatype = ARPHRD_ETHER;
    link.sll_pkttype = PACKET_OUTGOING;
    link.sll_halen = ETHER_ADDR_LEN;
    
    // Set the HW addy
    memcpy( link.sll_addr, ether_aton( "00:1e:4f:c3:26:ec" )->ether_addr_octet, ETHER_ADDR_LEN );
    
    // Get socket
    if ( (sock = socket( AF_PACKET, SOCK_RAW, link.sll_protocol )) == -1 )
    {
    fprintf( stderr, "Could not acquire socket file descriptor!\n" );
    perror( "socket() failed" );
    return -1;
    }
    
    // Bind socket
    if ( bind( sock, (struct sockaddr *)&link, sizeof(struct sockaddr_ll) ) == -1 )
    {
    fprintf( stderr, "Could not bind socket.\n" );
    perror( "bind() failed" );
    return -1;
    }
    
    // Stuff packet
    strncpy( (char*)buffer, "Hello World!", BUF_LEN );
    
    // Send packet on its way
    if ( (sent = sendto( sock, (char*)buffer, BUF_LEN, 0, (struct sockaddr *)&link, sizeof(struct sockaddr_ll) )) == -1 )
    {
    fprintf( stderr, "Could not send packet!\n" );
    perror( "send() failed" );
    return -1;
    }
    
    // Report
    fprintf( stdout, "Sent %d bytes...\n", sent );
    
    // Clean-up
    shutdown( sock, SHUT_RDWR );
    free( buffer );
    
    return 0;
 
}
