#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "socket.h"

#define BUFF_MAX_LEN 100

/* Notes on server
 * Current max buffer length for each transaction(listen or send) is 100 as defined by BUFF_MAX_LEN
 * Current packets being sended from server to client and vice versa is *assumes to be null terminated c-string*!!!
 */
class Server {
    private:
        Socket          endpoint;
        size_t          buffer_len;
        
    public:
        /* Core fcts */
        Server();
        Server(const char *);
        ~Server();
        bool bootup();
        bool listen(unsigned char *, ssize_t &);    // fct returns after one transmission
        bool send(const unsigned char *);           // fct returns after one transmission, TODO will need to specify where to send to
        bool shutdown();

        /* Interactions fcts that will implement our own protocol */
        // fcts goes here
};

#endif