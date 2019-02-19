#include <string>
#include "server.h"

/* Default constructor
 * no port number binded to socket created in this instance with this construtor
 * if chosen to use this constructor, it will need to bind the socket to a port number for this instance in order to be useful 
 */
Server::Server() {
    
}

/* Construtor to bind the server instance's socket to a pre-specify port number 
 * @param portnumber: The port number that will be used to bind socket
 */
Server::Server(int portnum) {

}

/* Binds a port number Server.socket in the server instance */
bool Server::connect_socket(int portnum) {

}

/* Unbinds the Socket in this instance of Server */
bool Server::close_socket() {
    
}

/* Called when When a session has been completed
 * the Socket's file descriptor from Server.socket will be closed, no longer refers to that fd
 */
/* bool Server::close() {
// This fct should be in Socket class
} */

/* Setter + Getter */
bool Server::send(std::string& msg) {

}

std::string Server::fetch() {

}