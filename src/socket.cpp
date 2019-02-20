#include <errno.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "socket.h"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* Default constructor */
Socket::Socket() {
    int fd = 0;
    //std::string sock_path = "socket_fd";
    int error_code = 0;
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
 * fd now has a physical file on file system somewhere
 * */
bool Socket::bindSocket(unsigned short sll_protocol_in, int sll_ifindex_in, unsigned char sll_halen_in, unsigned char* sll_addr_in, unsigned int sll_addr_in_len = 8) {
    sockaddr_ll myaddr;
    memset(&myaddr, 0, sizeof(sockaddr_ll));    //*memset(void *s, int c, size_t n)  -->  fills the first n bytes of the memory area pointed to by s with the constant byte c. Clears the structure

    myaddr.sll_family = AF_PACKET;
    myaddr.sll_protocol = ?;
    myaddr.sll_ifindex = ?;
    myaddr.sll_halen = ?;
    for( int i = 0; i < sll_addr_in_len; ++i ) {
        myaddr.sll_addr[i] = sll_addr_in[i];
    }

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
bool Socket::unlinkeSocket() {

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