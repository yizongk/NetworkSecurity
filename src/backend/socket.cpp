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
    this->error_code = 0;
    memset(&this->interface,0,INTERFACE_NAME_LEN);
    memcpy(&this->interface,"lo",2);
    // Set up address that is associated with this socket
    memset(&this->myaddr, 0, sizeof(this->myaddr)); //*memset(void *s, int c, size_t n)  -->  fills the first n bytes of the memory area pointed to by s with the constant byte c. Clears the structure
    this->myaddr.sll_family = AF_PACKET;            // Should always be AF_PACKET for RAW_SOCKET 
    this->myaddr.sll_protocol = htons(ETH_P_ALL);   // Takes in all packets
    this->myaddr.sll_ifindex = if_nametoindex(this->interface);   // 0 matches any interface, 1 is loopback device (not sure if it works), 2 is my local wlp59s0
}

/* Constructor 
 * @arg: name of the interface to bind the socket to, and the size of the interface name (The size should be <20)
 * changes the default interface name to be used in bind().
 */
Socket::Socket(const char *itf_name) {
    if(strlen(itf_name) >= 20) {
        handle_error("interface name length argument to constructor equals or exceeds 20. Ignore Errno message, doesn't apply to this crash.");
    }

    this->fd = -1;
    this->error_code = 0;
    memset(&this->interface,0,INTERFACE_NAME_LEN);
    memcpy(&this->interface,itf_name, strlen(itf_name));
    // Set up address that is associated with this socket
    memset(&this->myaddr, 0, sizeof(this->myaddr)); //*memset(void *s, int c, size_t n)  -->  fills the first n bytes of the memory area pointed to by s with the constant byte c. Clears the structure
    this->myaddr.sll_family = AF_PACKET;            // Should always be AF_PACKET for RAW_SOCKET 
    this->myaddr.sll_protocol = htons(ETH_P_ALL);   // Takes in all packets
    this->myaddr.sll_ifindex = if_nametoindex(this->interface);   // 0 matches any interface, 1 is loopback device (not sure if it works), 2 is my local wlp59s0
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
 * But our sockaddr_ll is already setted up in our constructor, so no need to worry about sll_protocol and sll_ifindex in this function
 * */
bool Socket::bindSocket() {
    if( bind( this->fd, (struct sockaddr *) &myaddr, sizeof(sockaddr_ll) ) == -1 ) {
        error_code = errno;
        handle_error("binding the socket");
        return false;
    }

    return true;
}

/* Send messages to address specified in sockaddr_ll through the socket
 * Message must be of type bytes AKA uint8_t, else sendto() will set errno
 *  */
bool Socket::sendMsg(const unsigned char *buf, const size_t len, const int flags) {
    ssize_t bytes = -1;   // The actualy len of contents sended out from the socket.  
                                // (>1) means the len of message. (==0) means len of msg is 0 or peer has performed an orderly shutdown. 
                                // (==-1) means an error occurred. In the event of an error, errno is set to indicate the error.
    uint8_t *ether_frame, *src_mac, *dst_mac;
    size_t frame_length;
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

    // Set the destination interface name: you need to fill these out
    strcpy(interface, this->interface);

    // Source IPv4 address: you need to fill this out
    strcpy (src_ip, "127.0.0.1");

    // Destination URL or IPv4 address: you need to fill this out
    strcpy (target, "localhost");

    // Use ioctl() to look up interface name and get its MAC address.
    memset (&ifr, 0, sizeof (ifr));
    memcpy (ifr.ifr_name, interface, sizeof (ifr.ifr_name));
    if (ioctl (this->fd, SIOCGIFHWADDR, &ifr) < 0) {
        delete ether_frame;
        delete src_mac;
        delete dst_mac;
        delete interface;
        delete target;
        delete src_ip;
        delete dst_ip;
        handle_error("ioctl() failed to get source MAC address ");
        return false;
    }

    // Copy source MAC address.
    memcpy (src_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));

    // Copy destination MAC address.
    memcpy (dst_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));

    // Fill out ethernet frame header.

    // Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (IP header + TCP header)
    //frame_length = 6 + 6 + 2 + IP4_HDRLEN + TCP_HDRLEN;
    //frame_length = 100;
    frame_length = len;

    // Destination and Source MAC addresses
    /* memcpy (ether_frame, dst_mac, 6 * sizeof (uint8_t));
    memcpy (ether_frame + 6, src_mac, 6 * sizeof (uint8_t)); */

    //strncpy( (char*)ether_frame, "Hello World!", 100 );
    //memcpy(ether_frame, "hello world!", 100 * sizeof (uint8_t));
    memcpy(ether_frame, buf, len*sizeof(uint8_t));

    // Report source MAC address to stdout.
    /* printf ("\tMAC address for src interface '%s' is '", interface);
    for (int i=0; i<6; i++) {
        printf ("%02x:", src_mac[i]);
    }
    printf("'\n"); */

    // Setting up the destination sockadd_ll
    struct sockaddr_ll dst_device;
    memset(&dst_device, 0, sizeof(struct sockaddr_ll));

    // Find interface index from interface name and store index in
    // struct sockaddr_ll dst_device, which will be used as an argument of sendto().
    if ((dst_device.sll_ifindex = if_nametoindex (interface)) == 0) {
        delete ether_frame;
        delete src_mac;
        delete dst_mac;
        delete interface;
        delete target;
        delete src_ip;
        delete dst_ip;
        handle_error("if_nametoindex() failed to obtain interface index ");
        return false;
    }
    /* printf ("\tIndex for interface %s is %i\n", interface, dst_device.sll_ifindex); */
    dst_device.sll_family = AF_PACKET;
    dst_device.sll_halen = ETH_ALEN;
    dst_device.sll_protocol = htons(ETH_P_ALL);
    
    memcpy (dst_device.sll_addr, dst_mac, ETH_ALEN * sizeof (uint8_t));

    if( ( bytes = sendto(this->fd,ether_frame,frame_length,0,(struct sockaddr *)&dst_device,sizeof(struct sockaddr_ll)) ) < 0 ) {
        error_code = errno;
        delete ether_frame;
        delete src_mac;
        delete dst_mac;
        delete interface;
        delete target;
        delete src_ip;
        delete dst_ip;
        handle_error("Socket::sendMsg()");
        return false;
    }
    /* printf("\tbytes send:'%lu' bytes\n\tether_frame:'%02x'\n\tframe_length:'%zu' bytes\n", bytes, ether_frame, frame_length); */

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

/* Receive messages from a socket (In bytes AKA uint8_t)
 * If  a  message is  too long to fit in the supplied buffer, excess bytes may be discarded depending on the type of socket the message is received from.
 * If no messages are available at the socket, the receive calls wait for a message to arrive, unless the socket is nonblocking
 * @buf: buffer    
 * @len: len of allocated size for buf
 * @bytes_in: the number of bytes recieved from socket
 * */
bool Socket::recvMsg(unsigned char *buf, const size_t len, ssize_t &bytes, sockaddr &src_addr, int &src_addr_len) {
    uint8_t *holder = new uint8_t[len];
    memset(holder,0,len * sizeof(uint8_t));
    memset(buf,0,len);    // Blank out the array.
    //ssize_t bytes = -1;   // The actualy len of contents recieved from socket.  
                                // (>1) means the len of message. (==0) means len of msg is 0 or peer has performed an orderly shutdown. 
                                // (==-1) means an error occurred. In the event of an error, errno is set to indicate the error.

    
    // src_addr will now contain information about where src comes from.
    if( ( bytes = recvfrom(this->fd,holder,len*sizeof(uint8_t),0,&src_addr,(socklen_t *)&src_addr_len) ) < 0 ) {
        error_code = errno;
        handle_error("Socket::recvMsg()");
        return false;
    }
    memcpy(buf,holder,len * sizeof(uint8_t));
    //printf("\tGot Message(%ld bytes):'%s'\n", bytes, buf);
    //printf("\tGot Message(%ld bytes):'%02x'\n", bytes, holder);
    // do something about memset(buf, holder)       // NOT THE JOB OF SOCKET! That's the job of server, leave socket as a way to transfer RAW data, all of the RAW data. Server and client (or a seperate protocol class will parse it.)

    return true;
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