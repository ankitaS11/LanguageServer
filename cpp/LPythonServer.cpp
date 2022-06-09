#include "LPythonServer.hpp"
#include "utilities.hpp"
#include "JSONRPC2Connection.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

#include <map>
#include <string>

struct handle_functions
{
  Logger log;
  JSONRPC2Connection* conn;
  handle_functions() {
    this->log = Logger();
    this->conn = new JSONRPC2Connection();

  }
  json serve_initialize(json request) {
    this->log.log("serve_initialized() called");
    json server_capabilities = {
      {"textDocumentSync", 2},
      {"documentSymbolProvider", true}
    };
    return {{"capabilities", server_capabilities}};
  }
  void serve_onSave(json request) {
        std::string uri = request["params"]["textDocument"]["uri"];
        int start_line = 2;
        int start_column = 0;
        int end_line = 3;
        int end_column = 10;
        std::string msg = "msg: Hi";
        json diag_results = {
            {"source", "source: Ankita"},
            {"range", {
                "start", {
                    {"line", start_line},
                    {"character", start_column},
                },
                "end", {
                    {"line", end_line},
                    {"character", end_column},
                },
            },
            {"message", msg},
            {"severity", 2},
        }
        };
        json message_send = {
          {"uri", uri},
          {"diagnostics", diag_results}
        };
        this->conn->send_notification(
            "textDocument/publishDiagnostics",
            message_send
          );

     this->log.log("serve_OnSave() called"); 
  }
};

typedef json (handle_functions::*json_handle)(json);
typedef void (handle_functions::*void_handle)(json);

typedef std::map<std::string, json_handle> json_mapfun;
typedef std::map<std::string, void_handle> void_mapfun;

void LPythonServer::handle(json request) {
    json_mapfun json_handler;
    void_mapfun void_handler;

    json_handler["initialize"] = &handle_functions::serve_initialize;
    void_handler["textDocument/didSave"] = &handle_functions::serve_onSave;

    std::string request_method = request["method"].get<std::string>();
    
    if (request.contains("id")) {
      json_mapfun::iterator x = json_handler.find(request_method);

      if (x != json_handler.end()) {
          handle_functions h;
          json resp = (h.*(x->second))(request);
          int rid = std::stoi(request["id"].dump());
          this->conn->write_message(rid, resp);
      }
    }
    else {
      void_mapfun::iterator x = void_handler.find(request_method);

      if (x != void_handler.end()) {
          handle_functions h;
          (h.*(x->second))(request);
      }
    }
}

void LPythonServer::run(std::string path) {
  while (this->running) {
    auto request = this->conn->read_message();
    this->handle(request);
  }
}