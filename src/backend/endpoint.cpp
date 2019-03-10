#include <string>
#include "endpoint.h"

/* Default constructor
 */
Endpoint::Endpoint() : buffer_len(BUFF_MAX_LEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)) {
     memset(&incom_src_addr,0,sizeof(incom_src_addr));
}

/*  Changes the default interface name that is used to bind the underlying socket
 */
Endpoint::Endpoint(const char *interface) : endpoint(interface), buffer_len(BUFF_MAX_LEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)) {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
}

/* Ensure Endpoint is closed.
 */
Endpoint::~Endpoint() {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->shutdown();
}

/* Opens, and bind the socket endpoint
 */
bool Endpoint::bootup() {
    return ( endpoint.openSocket() && endpoint.bindSocket() );
}

/* Listens for ONE incoming transmission. bytes is how much bytes was receieved.
 */
bool Endpoint::listen(unsigned char *buff, ssize_t &bytes) {
    return endpoint.recvMsg(buff, this->buffer_len, bytes, this->incom_src_addr, this->incom_src_addr_len);
}

/* Sends ONE outgoing transmission.
 * TODO: need to specify where to send to
 */
bool Endpoint::send(unsigned char *buffer) {
    return endpoint.sendMsg(buffer, this->buffer_len, 0);
}


/* Closes the socket, no more listening or sending...
 */
bool Endpoint::shutdown() {
    return endpoint.closeSocket();
}

/* append 
 */
bool append_hdr() {

}

// Add fcts here to 