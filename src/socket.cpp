#include <errno.h>
#include "socket.h"

/* If socket fails to construct, it means fd is set to -1 */
Socket::Socket() {
    fd = socket(AF_PACKET, SOCK_RAW, 0);

    if( fd == -1 ) {
        // Compare errno, and find out why.
        error_code = errno;
    }
}

/* Default destructor */
Socket::~Socket() {
    
}

/* Returns this instance of Socket has a file descriptor assigned to it */
bool Socket::isReady() {
    if( fd == -1 )
        return false;
    return true;
}

/* Returns the human-readable format of the specific error */
std::string Socket::getErrorStr() {
    return strerror(error_code);
}