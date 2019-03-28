#include <string>
#include <iostream>
#include <iomanip>
#include "endpoint.h"

using namespace std;
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
    struct shinyarmor_hdr income_hdr;

    while(true) {   // while it's not our port number
        memset(reciever,0,this->max_buffer_len);
        endpoint.recvMsg(reciever, this->max_buffer_len, bytes, this->incom_src_addr, this->incom_src_addr_len);
        memcpy(&income_hdr,reciever,sizeof(income_hdr));

        if(income_hdr.port_num == port_number) {
            break;
        }
    }

    memcpy(buff, reciever + sizeof(income_hdr), this->max_msg_len);

    delete[] reciever;
    return income_hdr.eof;
}

/* Sends ONE COMPLETE outgoing transmission (Our entire protocol is ran through once).
 * @argc port_number is use in the header defined in shinyarmor_hdr.h
 * @argc buf_size is only the maxium size of the payload. It does not define the maximun size of packet that is going out of this endpoint.
 */
bool Endpoint::send(unsigned char *buffer, const size_t buf_size, unsigned int port_number) {
    printf("DEBUG: in send()\n");
    // some codes here that detect if buffer is larger than buffer_len, if so break down into packets
    if( buf_size > this->max_msg_len ) {
        printf("Message too big...abort!\n");
        return false; 
    }

    this->build_packet(buffer, port_number);
    
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
bool Endpoint::build_packet(unsigned char* buf, unsigned int port_number) {
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
    hdr.port_num = port_number;

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

void Endpoint::run_protocol_send(unsigned char *buff, unsigned int port_number){
      
   std::string temp = "";
   ssize_t bytes = -1;
   unsigned char *incom_buf = new unsigned char[BUFLEN];
   memset(incom_buf,0,BUFLEN);
   cout << "Enter a message(less than 100 characters):" << endl;
   getline(cin, temp);
   if(temp.size() >= 100) {
       std::cout << "message too long(less than 100 characters)." << endl;
       std::cout << "-------------------------------------\n" << endl;
       //continue;
       return;
   }

   memset(buff,0,MAX_MSG_LEN);
   memcpy(buff,temp.c_str(),temp.size());
   this->send(buff, temp.size(), PORT_NUM); //first
   memset(buff,0,MAX_MSG_LEN);
     
   std::cout << "-------------------------------------\n" << endl;
   printf("\nListening...\n");
   this->listen(incom_buf, bytes, port_number); //first

}

void Endpoint::run_protocol_rcv(unsigned char *incom_buf, ssize_t &bytes, unsigned int port_number ){
      
     //while(true) {
        memset(incom_buf,0,BUFLEN);
        printf("\nListening...\n");
        cout<<"Server running...waiting for connection."<<endl;
        if( this->listen(incom_buf, bytes, port_number) ) { //first 
            
            
            printf("\nRequest Received...\n");
            //printf("%d. Received(%zu):\n'",i,bytes);
            std::string temp = "Acknowledged Request...";
            unsigned char *aBuffer = new unsigned char[BUFLEN];
            
            memset(aBuffer,0,BUFLEN);
            memcpy(aBuffer,temp.c_str(),temp.size());
            
            send(aBuffer, temp.size(), port_number); //first
            for(int j=0; j < bytes ;++j) {
                printf("%02x ",incom_buf[j]);
            }
            
            
            
            //aBuffer = "Acknowledged Request...";
            printf("'\n"); 

            std::cout << ". Received(" << bytes << " bytes):" << endl << "'";
            for(int j = 0; j < bytes; ++j) {
                //cout << std::hex << (int)incom_buf[j];
                cout << (char)incom_buf[j];
            }
            std::cout << std::dec << "'" << endl;

            // MAC address for interface 'lo' is: '00:00:00:00:00:00:'
            // MAC address for interface 'wlp59s0' is: '9c:b6:d0:ff:c8:75:  NOTE: This only applies to my own device(yi zong). Check out tool/getAllIndexAndMac.cpp to get your own Mac address'

            /* Extracting the Ethernet header as defined in linux/if_ether.h */
            /* struct ethhdr *eth = (struct ethhdr *)(incom_buf);
            printf("\nEthernet Header (The physical address, or in other word MAC address)\n");
            printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
            printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
            printf("\t|-Protocol : %d\n",eth->h_proto); */

            printf("---------------------------------------------\n");
            this->listen(incom_buf, bytes, port_number);

        }
    //}

}
