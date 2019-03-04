#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "socket.h"

#define BUFF_MAX_LEN 100

/* Notes on Client
 * Current max buffer length for each transaction(listen or send) is 100 as defined by BUFF_MAX_LEN
 * Current packets being sended from server to client and vice versa is *assumes to be null terminated c-string*!!!
 */
class Client {
    private:
        Socket          endpoint;
        size_t          buffer_len;
        
    public:
        /* Core fcts */
        Client();
        Client(const char *);
        ~Client();
        bool bootup();
        bool listen(unsigned char *, ssize_t &);    // fct returns after one transmission
        bool send(const unsigned char *);           // fct returns after one transmission, TODO will need to specify where to send to
        bool shutdown();

        /* Interactions fcts that will implement our own protocol - CLIENT SIDE */
        // fcts goes here
};

#endif