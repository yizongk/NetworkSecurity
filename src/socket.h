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

        bool isReady();
        std::string getErrorStr();
};

#endif