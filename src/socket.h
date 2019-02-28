#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <string.h>

class Socket {
    private:
        int fd;
        sockaddr_ll myaddr;
        int error_code;

    public:
        Socket();
        ~Socket();

        bool isValid() const;

        bool openSocket();
        bool bindSocket();    /* Work on hold, need to test if raw port can already receive data. */
        bool sendMsg(unsigned char *buf, size_t len, int flags);
        bool recvMsg(unsigned char *buf, size_t len);
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