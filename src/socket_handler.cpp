#include <iostream>

#include "socket_handler.h"


namespace seabottle {
    void* parse_request(void* socket_desc) {
        int sock = *(int*)socket_desc;
    }



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
        listen(this->socket_descriptor, this->max_queue);

        int conn_descriptor, temp = sizeof(struct sockaddr_in);
        std::string response;
        struct sockaddr_in client;

        while ( conn_descriptor = accept(this->socket_descriptor, (struct sockaddr*)&client, (socklen_t*)&temp) )  {
            pthread_t thread_id;
            // Is this right? -1? Or < 0?  I need to read more about this.
            if ( pthread_create(&thread_id, nullptr, parse_request, (void*)conn_descriptor) == -1 ) {
                perror("Failed to spawn thread.");
                return;
            }
            // Blocking call to garbage collect any thread resources before continuing.
            pthread_join(thread_id, NULL);
        }
        
    };


    void SocketServer::set_on_accept(std::string (*responding_func)()) {
        this->on_accept = responding_func;
    }
}
