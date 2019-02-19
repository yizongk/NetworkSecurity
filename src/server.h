#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "socket.h"

class Server {
    private:
        Socket          endpoint;
        
    public:
        /* Core setup fcts */
        Server();
        Server(int);
        ~Server();
        bool connect_socket(int);
        bool close_socket();

        /* Feature fcts */
        bool            send(std::string&);
        std::string     fetch();
};

#endif