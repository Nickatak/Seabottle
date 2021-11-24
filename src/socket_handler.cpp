#include "socket_handler.h"


namespace seabottle {
    bool is_end_http_req(std::vector<char>* data) {
        const char* termination_sequence = "\r\n\r\n";
        if (std::search(data->begin(), data->end(), termination_sequence, termination_sequence + strlen(termination_sequence)) != data->end() ) {
            return true;
        }

        return false;
    };

    void* recv_all(void* passed_struct) {
        // Cast type and dereference.
        PThreadPasser passer = *((PThreadPasser*)passed_struct);
        int chunk_size = 64;
        int offset_multiplier = 0;
        char* buffer_address;

        // Okay, so now I SHOULD be modifying the same memory as the PThreadPasser reference inside of the SocketServer::listen_continuously() function.
        passer.buf->resize(chunk_size);
        while(true) {
            // Calculate the char* address to start writing the recieved data into the buffer.
            buffer_address = passer.buf->data() + (chunk_size * offset_multiplier);

            // Read into the buffer and resize it as necessary.
            chunk_size = recv(passer.conn_descriptor, buffer_address, chunk_size, 0);
            passer.buf->resize(passer.buf->size() + chunk_size);

            if ( is_end_http_req(passer.buf) || (chunk_size == 0)) {
                // Exit the thread if we find the termination string OR we received no data.
                pthread_exit(nullptr);
            }
            offset_multiplier++;
        }

        // The computer should NEVER make it here -- but, I don't know, I'll leave it here just in case.
        pthread_exit(nullptr);
    };


    PThreadPasser::PThreadPasser(int conn, std::vector<char>* buf) {
        this->conn_descriptor = conn;
        this->buf = buf;
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
            throw std::runtime_error("Failed to bind to " + this->bind_addr + ':' + std::to_string(this->bind_port));
        }

        // Places the socket into listen mode with a max-queue of 3 attempted incoming connections (Note: This is a NON-blocking call).
        listen(this->socket_descriptor, this->MAX_QUEUE);

        int conn_descriptor, temp = sizeof(struct sockaddr_in);
        std::string response;
        struct sockaddr_in client;
 
        std::cout << "Listening..." << std::endl;
        while ( conn_descriptor = accept(this->socket_descriptor, (struct sockaddr*)&client, (socklen_t*)&temp) )  {
            pthread_t thread_id;
            PThreadPasser* passer = new PThreadPasser(conn_descriptor, new std::vector<char>());


            // Is this right? -1? Or < 0?  I need to read more about this.
            if ( pthread_create(&thread_id, nullptr, recv_all, (void*)passer) == -1 ) {
                perror("Failed to spawn thread.");
                return;
            }
            // Blocking call to garbage collect any thread resources before continuing.
            pthread_join(thread_id, NULL);

            this->on_accept(new std::string(passer->buf->begin(), passer->buf->end()));
        }
    };

    void SocketServer::set_on_accept(std::string (*responding_func)(std::string* raw_request)) {
        this->on_accept = responding_func;
    }
}
