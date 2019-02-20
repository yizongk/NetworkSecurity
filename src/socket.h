#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <string.h>

class Socket {
    private:
        int fd;
        //std::string sock_path;
        int error_code;
    public:
        Socket();
        ~Socket();

        bool openSocket();
        bool bindSocket(unsigned short sll_protocol_in, int sll_ifindex_in, unsigned char sll_halen_in, unsigned char* sll_addr_in, unsigned int sll_addr_in_len = 8);  // Before calling bindSocket, make sure ssl_addr_in is of size 8
        bool unlinkeSocket();
        bool closeSocket();









        std::string getErrorStr();

        /* ~For testing purposes~ */    //
        int getFd() {           //
            return this->fd;            //
        }                               //
        int getErrorCode() {    //
            return this->error_code;    //
        }                               //
        /* ~End of testing purposes~ */ //
};

#endif