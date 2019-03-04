#include <string>
#include "client.h"

/* Default constructor
 */
Client::Client() : buffer_len(BUFF_MAX_LEN) {
     
}

/*  Changes the default interface name that is used to bind the underlying socket
 */
Client::Client(const char *interface) : endpoint(interface), buffer_len(BUFF_MAX_LEN) {

}

/* Ensure Client is closed.
 */
Client::~Client() {
    this->shutdown();
}

/* Opens, and bind the socket endpoint
 */
bool Client::bootup() {
    return ( endpoint.openSocket() && endpoint.bindSocket() );
}

/* Listens for ONE incoming transmission. bytes is how much bytes was receieved.
 */
bool Client::listen(unsigned char *buff, ssize_t &bytes) {
    return endpoint.recvMsg(buff, this->buffer_len, bytes);
}

/* Sends ONE outgoing transmission.
 * TODO: need to specify where to send to
 */
bool Client::send(const unsigned char *buffer) {
    return endpoint.sendMsg(buffer, this->buffer_len, 0);
}


/* Closes the socket, no more listening or sending...
 */
bool Client::shutdown() {
    return endpoint.closeSocket();
}

// Add fcts here to 