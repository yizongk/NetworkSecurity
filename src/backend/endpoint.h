#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>
#include <map>
#include "socket.h"
#include "shinyarmor_hdr.h"
#include "constant.h"


/* Notes on Endpoint
 * 
 * This class is the level that we will be implemeenting our protocol. 
 * PURPOSE: Input of this class: if it's a to-send-message. Will break one message down into more than one packets if needed, append header to each packet, and send it off.
 *          Input of this class: if it's a received-message. Will collect one packet or multiple packets. Build the message, and return it by reference.
 * 
 * Current max buffer length for each transaction(listen or send) is 100 as defined by BUFF_MAX_LEN
 * Current packets being sended from Endpoint to client and vice versa is *assumes to be null terminated c-string*!!!
 */
class Endpoint {            // BIND THIS TO A PORT NUMBER AND LISTEN WILL GUARANTEE ONE MESSAGE FROM NEXT MSG WITH SPECIFIED PROT NUM
    private:
        Socket                          endpoint;
        size_t                          max_buffer_len;
        size_t                          max_msg_len;
        unsigned char                   *buf_to_send;
        struct sockaddr                 incom_src_addr;
        int                             incom_src_addr_len;
        unsigned int                    binded_port_num;
        std::map<unsigned int, bool>    visited_hdr_id;
        
        /* Feature fcts */
        bool build_packet(unsigned char*, unsigned int);
        unsigned int header_get_packet_id(unsigned char*);
        unsigned int header_get_packet_dst_port_num(unsigned char*);
        unsigned int header_get_packet_src_port_num(unsigned char*);
        bool header_is_eof(unsigned char*);
        bool packet_repeat(struct shinyarmor_hdr) const;
        unsigned int packet_id_generator();

    public:
        /* Core fcts */
        Endpoint();
        Endpoint(unsigned int);
        Endpoint(const char *);
        Endpoint(const char *, unsigned int);
        ~Endpoint();
        bool bootup();
        bool listen(unsigned char *, ssize_t &, struct shinyarmor_hdr&  = PLACE_HOLDER_SHINYARMOR);    // fct returns after one transmission in the correct port number sepcified in the header, it will be wrped in a while loop and return message of that psecific port
        bool send(unsigned char *, const size_t, const unsigned int);           // fct returns after one transmission, TODO will need to specify where to send to
        bool shutdown();
        unsigned int get_binded_port_num();
        void print_bytes_as(unsigned char *, ssize_t, std::string);
        
        
        /* Interactions fcts that will implement our own protocol */
        // fcts goes here

        bool run_protocol_send(unsigned char *, const size_t, unsigned int); 
        bool run_protocol_listen(unsigned char *, ssize_t);


};
               


#endif
