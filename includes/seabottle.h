#ifndef SEABOTTLE_H
#define SEABOTTLE_H

#include <vector>
#include <string>

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
    };
};


#endif