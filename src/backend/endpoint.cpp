#include <string>
#include "endpoint.h"

/* Default constructor
 */
Endpoint::Endpoint() : max_buffer_len(BUFF_MAX_LEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)) {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/*  Changes the default interface name that is used to bind the underlying socket
 */
Endpoint::Endpoint(const char *interface) : endpoint(interface), max_buffer_len(BUFF_MAX_LEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)) {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/* Ensure Endpoint is closed.
 */
Endpoint::~Endpoint() {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    delete this->buf_to_send;
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
    return endpoint.recvMsg(buff, this->max_buffer_len, bytes, this->incom_src_addr, this->incom_src_addr_len);
}

/* Sends ONE COMPLETE outgoing transmission (Our entire protocol is ran through once).
 */
bool Endpoint::send(unsigned char *buffer, const size_t buf_size) {
    // some codes here that detect if buffer is larger than buffer_len, if so break down into packets
    if( buf_size > (this->max_buffer_len - sizeof(struct shinyarmor_hdr)) ) {
        printf("Message too big...abort!");
        return false;
    }

    // If the message is small enough to send in one go.
    memcpy(this->buf_to_send, buffer, buf_size);
    return endpoint.sendMsg(this->buf_to_send, this->max_buffer_len, 0);
}


/* Closes the socket, no more listening or sending...
 */
bool Endpoint::shutdown() {
    return endpoint.closeSocket();
}

/* Append shinyarmor protocl header to buf, called only by Endpoint::send()
 */
bool Endpoint::build_packet(unsigned char* buf) {
    return true;
}

// Add fcts here to 