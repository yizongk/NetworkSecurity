#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits>       /* numeric_limits */
#include <future>       /* std::async, std::future */
#include <chrono>       /* std::chrono::milliseconds */
#include <functional>   /* std::bind */
#include "endpoint.h"

/* For default parameter value. For regression testing, where old src calls function that doesn't use to have certain parameter, due to being updated */
struct shinyarmor_hdr PLACE_HOLDER_SHINYARMOR;

/* Default constructor
 */
Endpoint::Endpoint() : max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(SERVER_PORT_NUM), visited_hdr_id() {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/* Binds a port number 
 */
Endpoint::Endpoint(unsigned int port_number) : max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(port_number), visited_hdr_id() {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/*  Changes the default interface name that is used to bind the underlying socket
 */
Endpoint::Endpoint(const char *interface) : endpoint(interface), max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(SERVER_PORT_NUM), visited_hdr_id() {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/*  Changes the default interface name that is used to bind the underlying socket. Also binds a port number
 */
Endpoint::Endpoint(const char *interface, unsigned int port_number) : endpoint(interface), max_msg_len(MAX_MSG_LEN), max_buffer_len(BUFLEN), incom_src_addr(), incom_src_addr_len(sizeof(incom_src_addr)), binded_port_num(port_number), visited_hdr_id() {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    this->buf_to_send = new unsigned char[this->max_buffer_len];
    memset(this->buf_to_send, 0, this->max_buffer_len);
}

/* Ensure Endpoint is closed.
 */
Endpoint::~Endpoint() {
    memset(&incom_src_addr,0,sizeof(incom_src_addr));
    delete[] this->buf_to_send;
    this->shutdown();
}

/* Opens, and bind the socket endpoint
 */
bool Endpoint::bootup() {
    return ( endpoint.openSocket() && endpoint.bindSocket() );
}

/* Listens for incoming transmissions. bytes is how much bytes was receieved.
 * @arg &income_hdr, will contain the header of the received packet.
 * @arg &buff will hold the payload of the packet.
 * Separates the header and payload. Return both by method of pass by reference.
 * 
 * Return condition: only returns when a incoming transmission that matches our binded port number
 * The returning bool is returned true, if the current incoming packet is completed successfully,
 * If returning bool is returned false, all pass-by-reference arg should not be consider valid.
 * 
 * Will ignore duplacated incoming packet
 * 
 */
bool Endpoint::listen(unsigned char *payload, ssize_t &bytes, struct shinyarmor_hdr& income_hdr) {
    
    unsigned char *reciever = new unsigned char[this->max_buffer_len];

    //printf("Endpoint Listening()\n");
    while(true) {   // while it's not our port number
        
        memset(reciever,0,this->max_buffer_len);
        endpoint.recvMsg(reciever, this->max_buffer_len, bytes, this->incom_src_addr, this->incom_src_addr_len);
        memcpy(&income_hdr,reciever,sizeof(income_hdr));
        if( this->packet_repeat(income_hdr) ) {
            continue;
        }
        
        if(income_hdr.dst_port_num == this->binded_port_num) {
            break;
        }
    }

    // Add packet id to list of all processed packet id
    visited_hdr_id[income_hdr.packet_id] = true;

    // Copy payload to buff
    memcpy(payload, reciever + sizeof(income_hdr), this->max_msg_len);

    struct shinyarmor_hdr hdr;
    memcpy(&hdr, reciever, sizeof(hdr));
    //printf("recieved message id and from_port_number: %u, %u\n", this->header_get_packet_id(reciever), this->header_get_packet_dst_port_num(reciever));

    delete[] reciever;
    return true;
}

/* Sends ONE COMPLETE outgoing transmission (Our entire protocol is ran through once).
 * @argc port_number is use in the header defined in shinyarmor_hdr.h
 * @argc buf_size is only the maxium size of the payload (size of the meaningful contents in buffer). It does not define the maximun size of packet that is going out of this endpoint.
 */
bool Endpoint::send(unsigned char *buffer, const size_t buf_size, unsigned int port_number) {
    // some codes here that detect if buffer is larger than buffer_len, if so break down into packets
    if( buf_size > this->max_msg_len ) {
        printf("Message too big...abort!\n");
        return false; 
    }

    this->build_packet(buffer, port_number);
    
    // If the message is small enough to send in one go.

    //printf("sended message id and to_port_number: %u, %u\n", this->header_get_packet_id(this->buf_to_send), this->header_get_packet_dst_port_num(this->buf_to_send));
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
    hdr.packet_id = this->packet_id_generator();
    hdr.eof = true;
    hdr.dst_port_num = port_number;
    hdr.src_port_num = this->binded_port_num;

    memcpy(this->buf_to_send,&hdr,sizeof(hdr));

    memcpy(this->buf_to_send + sizeof(hdr), buf, this->max_msg_len);

    return true;
}

/* 
 * Return packet ID base on shinyarmor_h
 * */
unsigned int Endpoint::header_get_packet_id(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.packet_id;
}

/* 
 * Return destination port number base on the shinyarmor_h
 */
unsigned int Endpoint::header_get_packet_dst_port_num(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.dst_port_num;
}

/* 
 * Return source port number base on the shinyarmor_h
 */
unsigned int Endpoint::header_get_packet_src_port_num(unsigned char* income_buf) {
    struct shinyarmor_hdr temp;
    memcpy(&temp, income_buf, sizeof(temp));
    return temp.src_port_num;
}

/* 
 * Returns bool eof base on shinyarmor_h
 */
bool Endpoint::header_is_eof(unsigned char* income_buf) {
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

/* 
 * Checks if incoming packet is a packet that is already been procesed, aka duplicated messages
 *  */
bool Endpoint::packet_repeat(struct shinyarmor_hdr a) const {
    //printf("packet checking repated\n");
    if ( visited_hdr_id.find(a.packet_id) == visited_hdr_id.end() ) {
        return false;
    }

    return true;
}

/* 
 *  Generates a random number in the range of unsigned int
 *  */
unsigned int Endpoint::packet_id_generator() {
    return ( rand() % std::numeric_limits<unsigned int>::max() );
}

/* 
 * Interprets bytes arrays as @param condition
 *  */
void Endpoint::print_bytes_as(unsigned char * bytes_arr, ssize_t bytes, std::string condition) {
    if(condition == "char") {
        for(int i = 0; i < bytes; ++i) {
            printf("%c", bytes_arr[i]);
        }

        return;
    } 

    if(condition == "hex") {
        for(int i = 0; i < bytes; ++i) {
            printf("%02X", bytes_arr[i]);
        }

        return;
    }

    printf("Please provide a valid condition.");

    return;
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

    printf("\nProtocol Send - Handshake Sending to server...\n");
    printf("send: 'Knock Knock...'\n");
    this->send(handshake, temp.length(), port_number); //first
     
    std::future<bool> fut = std::async( std::bind( &Endpoint::listen, this, std::ref(handshake), std::ref(handshake_bytes), std::ref(incoming_hdr) ) );
    printf("Protocol Send - Listening for replying ack from server...\n");
    std::chrono::milliseconds span(100);
    while( fut.wait_for(span) == std::future_status::timeout );
    bool status = fut.get();
    if(status == true) {
        printf("timeout did not happen\n");
    } else {
        printf("timeout!\n");
        return false;
    }
    /* if( !this->listen(handshake, handshake_bytes, incoming_hdr) ){      // M<AYBE TIME THIS CALL, don't need to modify listen, just need to time it
        // dosomething to stop attack
    }  */
    
    printf("Protocol Send - Ack Received from server(%zu bytes)\ngot: '", handshake_bytes);
    this->print_bytes_as(handshake,handshake_bytes,"char");
    printf("'\n");

    printf("Protocol Send - Payload Sending...\n");
    printf("send: '");
    this->print_bytes_as(buffer,buf_size,"char");
    printf("'\n");
    this->send(buffer, buf_size, port_number); //first

    reset_shinarmor_hdr(incoming_hdr);  // From shinyarmor_hdr.h
    memset(handshake,0,BUFLEN);
    handshake_bytes = -1;
    printf("Protocol Send - Listening for Payload Ack...\n");
    if( !this->listen(handshake, handshake_bytes, incoming_hdr) ) {
        // do something to stop attack
    }
    printf("Protocol Send - Payload Ack received from server(%zu bytes)\ngot: '", handshake_bytes);
    this->print_bytes_as(handshake,handshake_bytes,"char");
    printf("'\n");

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

    std::string temp = "Ack Ack Ack";
    std::string payload_temp = "Payload Ack";


    if( this->listen(handshake, handshake_bytes, incoming_hdr) ) { //first listen
        
        printf("Protocol Listen - Initial Handshake Request Received(%zu bytes) from client...\ngot: '", handshake_bytes);
        this->print_bytes_as(handshake, handshake_bytes, "char");
        printf("'\n");

        memcpy(handshake,temp.c_str(),temp.length());
        
        printf("Protocol Listen - Ack Sending to client...\nsend: '");
        this->print_bytes_as(handshake,handshake_bytes,"char");
        printf("'\n");
        this->send(handshake, temp.length(), incoming_hdr.src_port_num); //first
        
        reset_shinarmor_hdr(incoming_hdr);
        /* Now the protocol is over, recieve the actual message! */
        printf("Protocol Listen - Listening for Payload from client...\n");
        if( !this->listen(buff, recieved_buff_bytes, incoming_hdr) ) {
            //stop attack
        }
        printf("\nPayload (%zu bytes) next line\ngot: '", recieved_buff_bytes);
        this->print_bytes_as(buff, recieved_buff_bytes, "char");
        printf("'\n");


        printf("Protocol Listen - Sending payload ack to client...\nsend: '");
        memset(handshake,0,BUFLEN);
        memcpy(handshake,payload_temp.c_str(),payload_temp.length());
        this->print_bytes_as(handshake, payload_temp.length(), "char");
        printf("'\n");
        this->send(handshake, payload_temp.length(), incoming_hdr.src_port_num);
    }

    return true;
}
