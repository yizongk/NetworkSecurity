#include <errno.h>
#include <string>
#include <unistd.h>
#include "socket.h"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* Default constructor */
Socket::Socket() {
    
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
 * */
/* bool Socket::bindSocket() {
    if() {

    }

    return true;
} */

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