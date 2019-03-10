#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>
#include "socket.h"
#include "shinyarmor_hdr.h"

#define BUFF_MAX_LEN 20     /* BUG: magic number is 14 here, anything len below 14 will cause errno of invalid argument to system call for some reason. */

/* Notes on Endpoint
 * 
 * This class is the level that we will be implemeenting our protocol. 
 * PURPOSE: Input of this class: if it's a to-send-message. Will break one message down into more than one packets if needed, append header to each packet, and send it off.
 *          Input of this class: if it's a received-message. Will collect one packet or multiple packets. Build the message, and return it by reference.
 * 
 * Current max buffer length for each transaction(listen or send) is 100 as defined by BUFF_MAX_LEN
 * Current packets being sended from Endpoint to client and vice versa is *assumes to be null terminated c-string*!!!
 */
class Endpoint {
    private:
        Socket          endpoint;
        size_t          max_buffer_len;
        unsigned char   *buf_to_send;
        struct sockaddr incom_src_addr;
        int             incom_src_addr_len;
        
        /* Feature fcts */
        bool build_packet(unsigned char*);

    public:
        /* Core fcts */
        Endpoint();
        Endpoint(const char *);
        ~Endpoint();
        bool bootup();
        bool listen(unsigned char *, ssize_t &);    // fct returns after one transmission
        bool send(unsigned char *, const size_t);           // fct returns after one transmission, TODO will need to specify where to send to
        bool shutdown();

        
        /* Interactions fcts that will implement our own protocol */
        // fcts goes here
};

#endif