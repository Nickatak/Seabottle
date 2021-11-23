#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H
// sockaddr_in.
#include <arpa/inet.h>
// Exceptions.
#include <stdexcept>
// std::string.
#include <string>
// All the socket operations, read/write/listen/bind/etc.
#include <sys/socket.h>
// std::vector.
#include <vector>
// POSIX threads: pthread prefixed functions.
#include <pthread.h>


#include "socket_handler.h"


namespace seabottle {
    class SocketServer {
        private:
            int socket_descriptor, bind_port, max_queue = 3;
            struct sockaddr_in addr;
            std::string bind_addr;
            std::string (*on_accept)() = nullptr;
        public:
            SocketServer(std::string bind_addr, int port_number);
            /* Creates a new SocketServer object.
                :bind_arrd: The address you want to bind to (EG: "0.0.0.0" or "127.0.0.1").
                :port_number: The port number you want to bind to (EG: 8000, 80, or 8080).
            */
            void listen_continuously();
            /* Listens and accepts new connections continuously.

                At this point, SocketServer::set_on_accept(&your_func) needs to have been called
                otherwise this will throw a runtime error.
            */

            void set_on_accept(std::string (*responding_func)());
    };
};


#endif
