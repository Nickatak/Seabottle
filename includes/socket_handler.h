/* This is all the socket-level functions (sending/recieving/etc.). */
#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

// std::search()
#include <algorithm>
// sockaddr_in.
#include <arpa/inet.h>
// strlen().
#include <cstring>
#include <iostream>
// Exceptions.
#include <stdexcept>
// All the socket operations, read/write/listen/bind/etc.
#include <sys/socket.h>
#include <string>
#include <vector>
// POSIX threads: pthread prefixed functions.
#include <pthread.h>


namespace seabottle {
    bool is_end_http_req(std::vector<char>* data);
    void* recv_all(void* passed_struct);

    struct PThreadPasser {
        // Quick utility struct to pass stuff to the POSIX child-thread.
        int conn_descriptor;
        std::vector<char>* buf;
        PThreadPasser(int conn, std::vector<char>* buf);
    };


    class SocketServer {
        private:
            int socket_descriptor, bind_port, MAX_QUEUE = 3;
            struct sockaddr_in addr;
            std::string bind_addr;
            std::string (*on_accept)(std::string* raw_request) = nullptr;
            // Arbitrary max length in bytes (2MB) for an HTTP request?
            unsigned long MAX_BYTES = 2000000;
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

            void set_on_accept(std::string (*responding_func)(std::string* raw_request));
    };
};


#endif
