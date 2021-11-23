#include "seabottle.h"



namespace seabottle {
    App::App() {
        this->raw_server = new SocketServer("0.0.0.0", 8000);
    };

    void App::run() {
        this->raw_server->listen_continuously();
    }


}

