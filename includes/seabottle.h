/* This is the actual API our user wants to interact with. */
#ifndef SEABOTTLE_H
#define SEABOTTLE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "socket_handler.h"


namespace seabottle {
    struct Route {
        std::string path;
        std::string (*controller)();
    };


    class App {
        private:
            std::vector<Route> routes;
            SocketServer* raw_server;
        public:
            App();
            void run();
            void on(std::string path, std::string (*controller_func)());
    };
};


#endif