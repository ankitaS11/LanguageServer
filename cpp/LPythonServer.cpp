#include "LPythonServer.hpp"
#include "utilities.hpp"
#include "JSONRPC2Connection.hpp"
#include "nlohmann/json.hpp"

#include <map>
#include <string>

struct handle_functions
{  
   Logger log;
   handle_functions() {
     this->log = Logger();
   }
   void serve_initialize() { this->log.log("Initialized called"); }
   void serve_onSave() { this->log.log("OnSave called"); }
};

typedef void (handle_functions::*handle)(void);
typedef std::map<std::string, handle> map_fun;

void LPythonServer::handle(auto request) {
    this->log.log("Inside handle..");
    map_fun handler;
    handler["initialize"] = &handle_functions::serve_initialize;
    handler["textDocument/didSave"] = &handle_functions::serve_onSave;

    std::string request_method = request["method"];
    map_fun::iterator x = handler.find(request_method);

    if (x != handler.end()) {
        handle_functions h;
        (h.*(x->second))();
        std::cout << "Done";
    } 
}

void LPythonServer::run(std::string path) {
  while (this->running) {
    auto request = this->conn->read_message();
    // this->handle(request);
  }
}
