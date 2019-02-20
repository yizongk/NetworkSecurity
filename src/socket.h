#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <string.h>

class Socket {
    private:
        int fd;
        int error_code;
    public:
        Socket();
        ~Socket();

        bool openSocket();
        //bool bindSocket();
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