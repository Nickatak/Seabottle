#include <iostream>

#include "socket_handler.h"


namespace seabottle {
    struct PThreadPasser {
        // Quick utility struct to pass stuff to the POSIX child-thread.
        int conn_descriptor;
        std::vector<char>* buf;
        PThreadPasser(int conn, std::vector<char>* buf);
    };

    PThreadPasser::PThreadPasser(int conn, std::vector<char>* buf) {
        this->conn_descriptor = conn;
        this->buf = buf;
    };


    void* parse_request(void* passed_struct) {
        // Cast type and dereference.
        PThreadPasser passer = *((PThreadPasser*)passed_struct);

        // Okay, so now I SHOULD be modifying the same memory as the PThreadPasser reference inside of the SocketServer::listen_continuously() function.
        passer.buf->push_back('h');

        // Does this do what I think it does?  It sure as hell shuts up the compiler -- this might not be a great thing; I'll look at it later.
        return nullptr;
    };


    SocketServer::SocketServer(std::string bind_addr, int bind_port) {
        std::vector<char> first_line;

        // Describes a TCP Streaming IPv4 socket.
        this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

        if (socket_descriptor == -1) {
            // Failure to create socket.
            throw std::runtime_error("Failed to create TCP IPv4 socket.");
        }

        // TCP IPADDR:PORT
        this->addr.sin_family = AF_INET;
        this->addr.sin_addr.s_addr = inet_addr(bind_addr.c_str());
        this->addr.sin_port = htons(bind_port);

        this->bind_port = bind_port;
        this->bind_addr = bind_addr;
    };

    void SocketServer::listen_continuously() {
        // Error catch if the thread-handler isn't bound.
        //if (this->on_accept == nullptr) {
        //    throw std::runtime_error("on-accept function has not been set prior to attempting to listen.");
        //}

        // Bind the socket.
        if ( bind(socket_descriptor, (struct sockaddr*)&this->addr, sizeof(this->addr)) ) {
            throw std::runtime_error("Failed to bind to " + this->bind_addr + std::to_string(this->bind_port));
        }

        // Places the socket into listen mode with a max-queue of 3 attempted incoming connections (Note: This is a NON-blocking call).
        listen(this->socket_descriptor, this->MAX_QUEUE);

        int conn_descriptor, temp = sizeof(struct sockaddr_in);
        std::string response;
        struct sockaddr_in client;
 
        std::cout << "Listening..." << std::endl;
        while ( conn_descriptor = accept(this->socket_descriptor, (struct sockaddr*)&client, (socklen_t*)&temp) )  {
            pthread_t thread_id;
            PThreadPasser* passer = new PThreadPasser(conn_descriptor, new std::vector<char>(5000));


            // Is this right? -1? Or < 0?  I need to read more about this.
            if ( pthread_create(&thread_id, nullptr, parse_request, (void*)passer) == -1 ) {
                perror("Failed to spawn thread.");
                return;
            }
            // Blocking call to garbage collect any thread resources before continuing.
            pthread_join(thread_id, NULL);

            std::cout << std::string(passer->buf->begin(), passer->buf->end()) << std::endl;
        }
        
    };


    void SocketServer::set_on_accept(std::string (*responding_func)()) {
        this->on_accept = responding_func;
    }
}
