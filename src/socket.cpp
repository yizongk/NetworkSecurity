#include <errno.h>          // For error reporting for some system calls
#include <string>           // For string class
#include <cstring>          // For memcpy() and other useful nifty fct
#include <unistd.h>         // close()
#include <sys/socket.h>     // For socket system call
#include <sys/ioctl.h>      // For ioctl calls
#include <linux/if_ether.h> // For ETH_ALEN
#include <net/if.h>         // For if_nameindex, ifreq
#include <netinet/ip.h>     // struct ip and IP_MAXPACKET (which is 65535)
#include <netinet/ether.h>  // For struct ether_addr
#include <arpa/inet.h>      // For htons
#include "socket.h"

// Define some constants
//#define ETH_HDRLEN 14  // Ethernet header length
#define IP4_HDRLEN 20  // IPv4 header length
#define TCP_HDRLEN 20  // TCP header length, excludes options data

// Define error handling routine
#define handle_error(msg) \
    do { \
        perror(msg); \
        system( ( "errno " + std::to_string(errno) ).c_str() ); \
        exit(EXIT_FAILURE); \
    } while (0)

/* Constructor */
Socket::Socket() {
    this->fd = -1;
    memset(&this->myaddr, 0, sizeof(this->myaddr)); //*memset(void *s, int c, size_t n)  -->  fills the first n bytes of the memory area pointed to by s with the constant byte c. Clears the structure
    this->myaddr.sll_family = AF_PACKET;            // Should always be AF_PACKET for RAW_SOCKET 
    this->error_code = 0;
}


/* Destructor */
Socket::~Socket() {
    if( this->isValid() )  {
        this->closeSocket();
    }
}

/* 
 *  Returns a bool telling if this instance has a valid file descriptor.
 *  */
bool Socket::isValid() const {
    return this->fd !=-1;
}

/* If socket fails to construct, it means fd is set to -1 
*  This opens the socket to RAW communition
*/
bool Socket::openSocket() {
    if( ( this->fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)) ) == -1 ) {
        // Compare errno, and find out why.
        error_code = errno;
        handle_error("opening socket");
        return false;
    }

    return true;
}



/* Assigning a name to a socket 
 * In other word, giving the socket an address, and telling it the size (in bytes) of that specific address
 * fd now has a physical file on file system somewhere, probably lol, i don't know
 * for bind only sll_protocol and sll_ifindex are used (Taken from https://linux.die.net/man/7/packet)
 * sll_protocol, just pick a protocol from <linux/if_ether.h>
 * sll_ifindex refers to the physical or logical interface such as a network adapter or loopback device
 * */
bool Socket::bindSocket() {
    //memset(&(this->myaddr), 0, sizeof(sockaddr_ll));    

    this->myaddr.sll_family = AF_PACKET;
    this->myaddr.sll_protocol = htons(ETH_P_ALL);
    this->myaddr.sll_ifindex = if_nametoindex("wlp59s0");   // 0 matches any interface, 1 is loopback device (not sure if it works), 2 is my local wlp59s0

    if( bind( this->fd, (struct sockaddr *) &myaddr, sizeof(sockaddr_ll) ) == -1 ) {
        error_code = errno;
        handle_error("binding the socket");
        return false;
    }

    return true;
}

/* Send messages to address specified in sockaddr_ll through the socket
 *
 *  */
bool Socket::sendMsg(unsigned char *buf, size_t len, int flags) {
    memset(buf,0,len);    // Blank out the array.

    ssize_t bytes = -1;   // The actualy len of contents recieved from socket.  
                                // (>1) means the len of message. (==0) means len of msg is 0 or peer has performed an orderly shutdown. 
                                // (==-1) means an error occurred. In the event of an error, errno is set to indicate the error.
    uint8_t *ether_frame, *src_mac, *dst_mac;
    int frame_length;
    char *interface, *target, *dst_ip, *src_ip;
    struct ifreq ifr;

    // Giving some dynamically allocated mem to variables.
    ether_frame = new uint8_t[IP_MAXPACKET];
    src_mac = new uint8_t[6];
    dst_mac = new uint8_t[6];
    interface = new char[40];
    target = new char[40];
    src_ip = new char[INET_ADDRSTRLEN];
    dst_ip = new char[INET_ADDRSTRLEN];

    // Set destination MAC address: you need to fill these out
    /* dst_mac[0] = 0x9c;              // 9c:b6:d0:ff:c8:75: for wlp59s0
    dst_mac[1] = 0xb6;              // 00:00:00:00:00:00: for lo
    dst_mac[2] = 0xd0;
    dst_mac[3] = 0xff;
    dst_mac[4] = 0xc8;
    dst_mac[5] = 0x75; */

    // Set the interface name: you need to fill these out
    strcpy(interface, "wlp59s0");

    // Source IPv4 address: you need to fill this out
    strcpy (src_ip, "127.0.0.1");

    // Destination URL or IPv4 address: you need to fill this out
    strcpy (target, "localhost");

    // Use ioctl() to look up interface name and get its MAC address.
    memset (&ifr, 0, sizeof (ifr));
    memcpy (ifr.ifr_name, interface, sizeof (ifr.ifr_name));
    if (ioctl (this->fd, SIOCGIFHWADDR, &ifr) < 0) {
        handle_error("ioctl() failed to get source MAC address ");
        return false;
    }

    // Copy source MAC address.
    memcpy (src_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));

    // Copy destination MAC address.
    memcpy (dst_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));

    // Fill out ethernet frame header.

    // Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (IP header + TCP header)
    frame_length = 6 + 6 + 2 + IP4_HDRLEN + TCP_HDRLEN;

    // Destination and Source MAC addresses
    memcpy (ether_frame, dst_mac, 6 * sizeof (uint8_t));
    memcpy (ether_frame + 6, src_mac, 6 * sizeof (uint8_t));

    //strncpy( (char*)ether_frame, "Hello World!", 100 );
    memcpy(ether_frame + 12, "hello world!", 100);

    // Report source MAC address to stdout.
    /* printf ("\tMAC address for interface %s is ", interface);
    for (int i=0; i<6; i++) {
        printf ("%02x:", src_mac[i]);
    }
    printf("\n"); */

    // Setting up the sockadd_ll
    struct sockaddr_ll device;
    memset(&device, 0, sizeof(struct sockaddr_ll));

    // Find interface index from interface name and store index in
    // struct sockaddr_ll device, which will be used as an argument of sendto().
    if ((device.sll_ifindex = if_nametoindex (interface)) == 0) {
        handle_error("if_nametoindex() failed to obtain interface index ");
        return false;
    }
    /* printf ("\tIndex for interface %s is %i\n", interface, device.sll_ifindex); */
    device.sll_family = AF_PACKET;
    device.sll_halen = ETH_ALEN;
    device.sll_ifindex = if_nametoindex("wlp59s0");
    device.sll_protocol = htons(ETH_P_ALL);
    //struct ether_addr *ea = NULL;
    //ea = ether_aton("9c:b6:d0:ff:c8:75");
    //ea = ether_aton("00:00:00:00:00:00");
    /* if( !ea ) {
        errno = 6;
        error_code = errno;
        handle_error("Socket::getfrom()->error converting Hardware addr. ERRNO doens't make sense here. It's okay");
        return false;
    } */
    //memcpy(out_addr.sll_addr, ea->ether_addr_octet, ETH_ALEN);
    memcpy (device.sll_addr, src_mac, ETH_ALEN * sizeof (uint8_t));
    
    /* printf("sll_family(unsigned short): '%u'\n", device.sll_family);
    printf("sll_protocol(__be16): '%u'\n", device.sll_protocol);
    printf("sll_ifindex(int): '%u'\n", device.sll_ifindex);
    printf("sll_hatype(unsigned short): '%u'\n", device.sll_hatype);
    printf("sll_pkttype(unsigned char): '%u'\n", device.sll_pkttype);
    printf("sll_halen(unsigned char): '%u'\n", device.sll_halen);
    printf("sll_addr(unsigned char)[]: '");
    for(int i = 0; i < 8; ++i) {
        printf("%02x:", device.sll_addr[i]);
    }
    printf("'\n");  */

    // Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (IP header + TCP header)
    //frame_length = 6 + 6 + 2 + IP4_HDRLEN + TCP_HDRLEN;

    //sendto(fd,out,4,0,NULL,0);       //ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
    if( ( bytes = sendto(this->fd,ether_frame,frame_length,0,(struct sockaddr *)&device,sizeof(struct sockaddr_ll)) ) < 0 ) {
    //if( ( act_buf_len = sendto(this->fd,out,4,0,(struct sockaddr *)&this->myaddr, sizeof(this->myaddr)) ) < 0 ) { 
        error_code = errno;
        handle_error("Socket::sendMsg()");
        return false;
    }
    //printf("\tbytes send:'%lu'\n\tether_frame:'%02x'\n\tframe_length:'%u'\n", bytes, ether_frame, frame_length);

    // Clean up dynamically allocated resources
    delete ether_frame;
    delete src_mac;
    delete dst_mac;
    delete interface;
    delete target;
    delete src_ip;
    delete dst_ip;

    return true;
}

/* Receive messages from a socket
 * If  a  message is  too long to fit in the supplied buffer, excess bytes may be discarded depending on the type of socket the message is received from.
 * If no messages are available at the socket, the receive calls wait for a message to arrive, unless the socket is nonblocking
 * */
bool Socket::recvMsg(unsigned char *buf, size_t len) {
    memset(buf,0,len);    // Blank out the array.
    ssize_t bytes = -1;   // The actualy len of contents recieved from socket.  
                                // (>1) means the len of message. (==0) means len of msg is 0 or peer has performed an orderly shutdown. 
                                // (==-1) means an error occurred. In the event of an error, errno is set to indicate the error.

    
    if( ( bytes = recvfrom(this->fd,buf,len,0,NULL,NULL) ) < 0 ) {
        error_code = errno;
        handle_error("Socket::recvMsg()");
        return false;
    }
    printf("\tGot Message:'%s'\n", buf);
printf("\trecvfrom() done.\n");

    /* ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen); */

    return true;
}

/* delete a name (the fd that was binded) and possibly the file it refers to 
 * in other word, unbinding it
 * */
bool Socket::unlinkSocket() {

}

/* Release the resource associated with file descriptor of this socket */
bool Socket::closeSocket() {
    if( close(this->fd) != 0 ) {
        error_code = errno;
        return false;
    }
    return true;
}

/* Returns the human-readable format of the specific error */
std::string Socket::getErrorStr() {
    return strerror(error_code);
}