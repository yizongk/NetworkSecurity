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
    if( ( this->fd = socket(AF_PACKET, SOCK_RAW, ETH_P_IP) ) == -1 ) {
        // Compare errno, and find out why.
        error_code = errno;
        handle_error("opening socket");
        return false;
    }

    return true;
}

/* Receive messages from a socket
 * If  a  message is  too long to fit in the supplied buffer, excess bytes may be discarded depending on the type of socket the message is received from.
 * If no messages are available at the socket, the receive calls wait for a message to arrive, unless the socket is nonblocking
 * */
bool Socket::getMsg(void *buf, size_t len, int flags) {
    memset(buf,0,len);    // Blank out the array.
printf("memset() done.\n");
    ssize_t act_buf_len = -1;   // The actualy len of contents recieved from socket.  
                                // (>1) means the len of message. (==0) means len of msg is 0 or peer has performed an orderly shutdown. 
                                // (==-1) means an error occurred. In the event of an error, errno is set to indicate the error.

    unsigned char *out = new unsigned char[4];
    //sendto(fd,out,4,0,NULL,0);       //ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
    if( ( act_buf_len = sendto(fd,out,4,0,NULL,0) ) < 0 ) {
        error_code = errno;
        handle_error("Socket::getfrom() 1");
        return false;
    }
    delete[] out;

    if( ( act_buf_len = recvfrom(fd,buf,len,flags,NULL,NULL) ) < 0 ) {
        error_code = errno;
        handle_error("Socket::getfrom() 2");
        return false;
    }
printf("recvfrom() done.\n");

    /* ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen); */

    return true;
}

/* Assigning a name to a socket 
 * In other word, giving the socket an address, and telling it the size (in bytes) of that specific address
 * fd now has a physical file on file system somewhere, probably lol, i don't know
 * for bind only sll_protocol and sll_ifindex are used (Taken from https://linux.die.net/man/7/packet)
 * sll_protocol, just pick a protocol from <linux/if_ether.h>
 * sll_ifindex refers to the physical or logical interface such as a network adapter or loopback device
 * */
bool Socket::bindSocket(unsigned short sll_protocol_in, int sll_ifindex_in) {
    //memset(&(this->myaddr), 0, sizeof(sockaddr_ll));    

    this->myaddr.sll_protocol = sll_protocol_in;
    this->myaddr.sll_ifindex = sll_ifindex_in;

    if( bind( this->fd, (struct sockaddr *) &myaddr, sizeof(sockaddr_ll) ) == -1 ) {
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