#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <string.h>

#define INTERFACE_NAME_LEN 20

// Designed to be a RAW socket
class Socket {
    private:
        int fd;                 // File descriptor for the socket
        sockaddr_ll myaddr;     // The address information for this socket
        int error_code;         // Error code produced by this class, will contain the same as errno produced by functions that is associated with this class
        char interface[INTERFACE_NAME_LEN];     // The interface name that this socket interacts with, defaults is loopback which is 'lo'. For tool to find your interface, refer to tool/getAllIndexAndMac.cpp

    public:
        Socket();
        //Socket(char*);
        ~Socket();

        bool isValid() const;

        bool openSocket();
        bool bindSocket();    /* Work on hold, need to test if raw port can already receive data. */
        bool sendMsg(const unsigned char *buf, const size_t len, const int flags);
        bool recvMsg(unsigned char *buf, const size_t len, ssize_t &bytes_in);
        bool unlinkSocket();        /* Need work */
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