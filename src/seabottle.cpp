#include "seabottle.h"


namespace seabottle {
    std::string parse_request(std::string *raw_request) {
        std::map<std::string, std::string> headers;
        std::vector<std::string> lines;
        size_t pos = 0;
        while ( (pos = raw_request->find("\r\n")) != std::string::npos ) {
            std::cout << raw_request->substr(0, pos) << std::endl;
            lines.push_back(raw_request->substr(0, pos));
            raw_request->erase(0, pos + 1);
        }

        std::cout << lines[0] << std::endl;
        return "testing complete";
    };


    App::App() {
        this->raw_server = new SocketServer("0.0.0.0", 8000);
    };

    void App::on(std::string path, std::string (*controller_func)()) {
        this->routes.push_back(
            Route {path, controller_func}
        );
    };

    void App::run() {
        this->raw_server->set_on_accept(&parse_request);
        this->raw_server->listen_continuously();
    };
}

