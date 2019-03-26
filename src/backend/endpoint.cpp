#include <string>
#include "endpoint.h"

/* Default constructor
 */
Endpoint::Endpoint() : max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)) {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/*  Changes the default interface name that is used to bind the underlying socket
 */
Endpoint::Endpoint(const char *interface) : endpoint(interface), max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)) {
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

/* Listens for incoming transmissions. bytes is how much bytes was receieved.
 * @argc port_number is use to filter incoming packets with header defined in shinyarmor_hdr.h
 * Strips off the header and return only the message part to buff that is pass by reference.
 * Return condition: only returns when a incoming transmission that matches our port number defined int constant.h
 * After it returns, buff will hold the message of the transmmission, the header is not part of it (it's been striped away)
 * 
 * The returning bool is returned true, if the current incoming packet is an end-of-file,
 * meaning the current incoming packet is the last of the series of packet that came in
 */
bool Endpoint::listen(unsigned char *buff, ssize_t &bytes, unsigned int port_number) {
    
    // SOMEHOW STIRP OFF THE HEADER HERE
    unsigned char *reciever = new unsigned char[this->max_buffer_len];
    struct shinyarmor_hdr hdr;

    while(true) {   // while it's not our port number
        memset(reciever,0,this->max_buffer_len);
        endpoint.recvMsg(reciever, this->max_buffer_len, bytes, this->incom_src_addr, this->incom_src_addr_len);
        memcpy(&hdr,reciever,sizeof(hdr));

        if(hdr.port_num == PORT_NUM) {
            break;
        }
    }

    memcpy(buff, reciever + sizeof(hdr), this->max_msg_len);

    delete[] reciever;
    return hdr.eof;
}

/* Sends ONE COMPLETE outgoing transmission (Our entire protocol is ran through once).
 * @argc port_number is use in the header defined in shinyarmor_hdr.h
 */
bool Endpoint::send(unsigned char *buffer, const size_t buf_size, unsigned int port_number) {
    printf("DEBUG: in send()\n");
    // some codes here that detect if buffer is larger than buffer_len, if so break down into packets
    if( buf_size > this->max_msg_len ) {
        printf("Message too big...abort!\n");
        return false; 
    }

    this->build_packet(buffer);
    
    // If the message is small enough to send in one go.
    /* memcpy(this->buf_to_send, buffer, this->max_buffer_len); */
    printf("DEBUG: buf_to_send:");
    for( int i = 0; i < this->max_buffer_len; ++i ) {
        printf("%1x",this->buf_to_send[i]);
    }
    printf("\n");
    printf("DEBUG: port_num: '%u' and eof: '%d'\n", this->get_packet_port_num(this->buf_to_send), this->is_eof(this->buf_to_send) );
    return endpoint.sendMsg(this->buf_to_send, this->max_buffer_len, 0);
}


/* Closes the socket, no more listening or sending...
 */
bool Endpoint::shutdown() {
    return endpoint.closeSocket();
}

/* Append shinyarmor protocl header to buf, called only by Endpoint::send()
 * @argc buf is the message
 * builds the packet and store it in Endpoint::buf_to_send
 */
bool Endpoint::build_packet(unsigned char* buf) {
    printf("DEBUG: In building_packet()\n");
    printf("\tDEBUG: total_msg_len = %zu. Glboal buflen = %zu\n", this->max_buffer_len, BUFLEN);

    printf("\tDEBUG: Buf:");
    for( int i = 0; i < this->max_msg_len; ++i ) {
        printf("%1x",buf[i]);
    }
    printf("\n");

    /* printf("DEBUG: Bufsdf:");
    for( int i = 0; i < this->max_msg_len; ++i ) {
        printf("%d",buf[200]);                      // THIS IS SUPPOSE TO CAUSE SEG FAULT BUT IT"S NOT!!! 200 is more than the index we allocated for buf!
    }
    printf("\n"); */

    struct shinyarmor_hdr hdr;
    hdr.eof = true;
    hdr.port_num = PORT_NUM;

    memcpy(this->buf_to_send,&hdr,sizeof(hdr));
    printf("\tDEBUG: hdr(len: '%zu'):", sizeof(hdr));
    for( int i = 0; i < sizeof(hdr); ++i ) {
        printf("%1x",this->buf_to_send[i]);
    }
    printf("\n");

    memcpy(this->buf_to_send + sizeof(hdr), buf, this->max_msg_len);

    printf("\tDEBUG: hdr + Buf:");
    for( int i = 0; i < this->max_buffer_len; ++i ) {
        printf("%1x",this->buf_to_send[i]);
    }
    printf("\n");

    printf("\tDEBUG: buf_to_send:");
    for( int i = 0; i < this->max_buffer_len; ++i ) {
        printf("%1x",this->buf_to_send[i]);
    }
    printf("\n");

    return true;
}

/* 
 * Return port number base on the shinyarmor_h
 */
unsigned int Endpoint::get_packet_port_num(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.port_num;
}

/* 
 * Returns bool eof base on shinyarmor_h
 */
bool Endpoint::is_eof(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.eof;
}

// Add fcts here to 

void Endpoint::run_protocol_send(){
      
     

}

void Endpoint::run_protocol_rcv(){
      
     

}
