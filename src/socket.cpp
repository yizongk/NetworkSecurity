#include <errno.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include "socket.h"

#define handle_error(msg) \
    do { \
    perror(msg); \
    printf("errno: %d\n", errno); \
    system( ( "errno " + std::to_string(errno) ).c_str() ); \
    exit(EXIT_FAILURE); \
    } while (0)

/* Default constructor */
Socket::Socket() {
    this->fd = 0;
    this->myaddr.sll_family = AF_PACKET;        // unsigned short
    this->myaddr.sll_protocol = 0;               // unsigned short
    this->myaddr.sll_ifindex = 0;                // int 
    this->myaddr.sll_hatype = 0;                 // unsigned short
    this->myaddr.sll_pkttype;                // unsigned char
    this->myaddr.sll_halen;                  // unsigned char
    this->myaddr.sll_addr;;                   // unsigned char[8]
    this->error_code = 0;
}


/* Default destructor */
Socket::~Socket() {
    
}

/* If socket fails to construct, it means fd is set to -1 
*  This opens the socket to RAW communition
*/
bool Socket::openSocket() {
    if( ( this->fd = socket(AF_PACKET, SOCK_RAW, 0) ) == -1 ) {
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
 * */
bool Socket::bindSocket(unsigned short sll_protocol_in, int sll_ifindex_in) {
    memset(&(this->myaddr), 0, sizeof(sockaddr_ll));    //*memset(void *s, int c, size_t n)  -->  fills the first n bytes of the memory area pointed to by s with the constant byte c. Clears the structure
    
    this->myaddr.sll_protocol = sll_protocol_in;
    this->myaddr.sll_ifindex = sll_ifindex_in;

    if( bind( this->fd, (struct sockaddr *) &myaddr, sizeof(sockaddr_ll) ) != 0 ) {
        error_code = errno;
        handle_error("binding the socket");
        return false;
    }

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