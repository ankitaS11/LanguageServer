#ifndef LPYTHON_SERVER_HPP
#define LPYTHON_SERVER_HPP

#include <string>
#include "JSONRPC2Connection.hpp"
#include "utilities.hpp"

class LPythonServer {
    private:
        Logger log;
    public:
        bool running = false;
        JSONRPC2Connection* conn;
        LPythonServer() {
            this->log = Logger();
            this->running = true;
            this->conn = new JSONRPC2Connection();
        }

        void run(std::string);

        void handle(auto);
};

#endif