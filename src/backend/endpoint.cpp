#include <string>
#include "endpoint.h"

/* For default parameter value. For regression testing, where old src calls function that doesn't use to have certain parameter, due to being updated */
struct shinyarmor_hdr PLACE_HOLDER_SHINYARMOR;

/* Default constructor
 */
Endpoint::Endpoint() : max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(SERVER_PORT_NUM) {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/* Binds a port number 
 */
Endpoint::Endpoint(unsigned int port_number) : max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(port_number) {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/*  Changes the default interface name that is used to bind the underlying socket
 */
Endpoint::Endpoint(const char *interface) : endpoint(interface), max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(SERVER_PORT_NUM) {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/*  Changes the default interface name that is used to bind the underlying socket. Also binds a port number
 */
Endpoint::Endpoint(const char *interface, unsigned int port_number) : endpoint(interface), max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(port_number) {
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
 * Separates the header and payload. Return both by method of pass by reference.
 * Return condition: only returns when a incoming transmission that matches our port number defined int constant.h
 * After it returns, buff will hold the message of the transmmission, the header is not part of it (it's been striped away)
 * 
 * The returning bool is returned true, if the current incoming packet is an end-of-file,
 * meaning the current incoming packet is the last of the series of packet that came in
 */
bool Endpoint::listen(unsigned char *buff, ssize_t &bytes, struct shinyarmor_hdr& income_hdr) {
    
    // SOMEHOW STIRP OFF THE HEADER HERE
    unsigned char *reciever = new unsigned char[this->max_buffer_len];

    while(true) {   // while it's not our port number
        memset(reciever,0,this->max_buffer_len);
        endpoint.recvMsg(reciever, this->max_buffer_len, bytes, this->incom_src_addr, this->incom_src_addr_len);
        memcpy(&income_hdr,reciever,sizeof(income_hdr));

        if(income_hdr.dst_port_num == this->binded_port_num) {
            break;
        }
    }

    memcpy(buff, reciever + sizeof(income_hdr), this->max_msg_len);

    delete[] reciever;
    return income_hdr.eof;
}

/* Sends ONE COMPLETE outgoing transmission (Our entire protocol is ran through once).
 * @argc port_number is use in the header defined in shinyarmor_hdr.h
 * @argc buf_size is only the maxium size of the payload (size of the meaningful contents in buffer). It does not define the maximun size of packet that is going out of this endpoint.
 */
bool Endpoint::send(unsigned char *buffer, const size_t buf_size, unsigned int port_number) {
    // some codes here that detect if buffer is larger than buffer_len, if so break down into packets
    if( buf_size > this->max_msg_len ) {
        printf("Message too big...abort!\n");
        //return false; 
        for(int j = 0; j < buf_size; ++j) {
            std::cout << std::hex << (int)buffer[j];
            std::cout << (char)buffer[j];
        }

    }

    this->build_packet(buffer, port_number);
    
    // If the message is small enough to send in one go.

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
bool Endpoint::build_packet(unsigned char* buf, unsigned int port_number) {

    /* printf("DEBUG: Bufsdf:");
    for( int i = 0; i < this->max_msg_len; ++i ) {
        printf("%d",buf[200]);                      // THIS IS SUPPOSE TO CAUSE SEG FAULT BUT IT"S NOT!!! 200 is more than the index we allocated for buf!
    }
    printf("\n"); */

    struct shinyarmor_hdr hdr;
    hdr.eof = true;
    hdr.dst_port_num = port_number;
    hdr.src_port_num = this->binded_port_num;

    memcpy(this->buf_to_send,&hdr,sizeof(hdr));

    memcpy(this->buf_to_send + sizeof(hdr), buf, this->max_msg_len);

    return true;
}

/* 
 * Return destination port number base on the shinyarmor_h
 */
unsigned int Endpoint::get_packet_dst_port_num(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.dst_port_num;
}

/* 
 * Return source port number base on the shinyarmor_h
 */
unsigned int Endpoint::get_packet_src_port_num(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.src_port_num;
}

/* 
 * Returns bool eof base on shinyarmor_h
 */
bool Endpoint::is_eof(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.eof;
}

/* 
 * Returns the port number that is binded to this endpoint
 */
unsigned int Endpoint::get_binded_port_num() {
    return this->binded_port_num;
}

// Add fcts here to 
/* Protocol for send:
 * ...
 * Need to identify where we can return false in this function. 
 */
bool Endpoint::run_protocol_send(unsigned char *buffer, const size_t buf_size, const unsigned int port_number) {
      
    ssize_t handshake_bytes = -1;
    unsigned char *handshake = new unsigned char[BUFLEN];
    memset(handshake,0,BUFLEN);
    struct shinyarmor_hdr incoming_hdr;

    std::string temp = "Knock Knock...";
    memcpy(handshake,temp.c_str(),temp.length());

    printf("\nProtocol - Handshake Sending...\n");
    this->send(handshake, temp.length(), port_number); //first
     
    printf("Protocol - Listening...\n");
    this->listen(handshake, handshake_bytes, incoming_hdr); //first
    printf("Protocol - Hanshake Received(%zu bytes)\n'", handshake_bytes);
    for(int j = 0; j < handshake_bytes; ++j) {
        //cout << std::hex << (int)incom_buf[j];
        printf("%c", handshake[j]);
    }
    printf("'\n");

    printf("Protocol - Payload Sending...\n");
    this->send(buffer, buf_size, port_number); //first

    return true;
}

/* Protocol for listen:
 * ...
 * Need to identify where we can return false in this function. 
 */
bool Endpoint::run_protocol_listen(unsigned char *buff, ssize_t recieved_buff_bytes) {
    ssize_t handshake_bytes = -1;
    unsigned char *handshake = new unsigned char[BUFLEN];
    memset(handshake,0,BUFLEN);
    struct shinyarmor_hdr incoming_hdr;

    std::string temp = "Acknowledged Request...";
      

    printf("\nListening...\n");
    printf("Server running...waiting for connection.\n");

    if( this->listen(handshake, handshake_bytes, incoming_hdr) ) { //first   
        
        printf("\nProtocol - Handshake Request Received(%zu bytes)...\n'", handshake_bytes);
        for(int j = 0; j < handshake_bytes; ++j) {
            printf("%c", handshake[j]);
        }
        printf("'\n");
        
        memcpy(handshake,temp.c_str(),temp.length());
        
        printf("Protocol - Handshake Sending...\n");
        this->send(handshake, temp.length(), incoming_hdr.src_port_num); //first
        
        /* Now the protocol is over, recieve the actual message! */
        printf("Protocol - Recieving Payload...\n");
        this->listen(buff, recieved_buff_bytes, incoming_hdr);

        printf("---------------------------------------------\n");

    }

    return true;
}
