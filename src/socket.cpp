#include <errno.h>
#include <string>
#include <unistd.h>
#include "socket.h"

/* Default constructor */
Socket::Socket() {
    
}

/* Default destructor */
Socket::~Socket() {
    
}

/* If socket fails to construct, it means fd is set to -1 */
bool Socket::openSocket() {
    this->fd = socket(AF_PACKET, SOCK_RAW, 0);

    if( this->fd == -1 ) {
        // Compare errno, and find out why.
        error_code = errno;
        return false;
    }

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