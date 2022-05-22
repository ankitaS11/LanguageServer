#include "JSONRPC2Connection.hpp"
#include <string>
#include <map>

class LPythonServer {
  public:
    JSONRPC2Connection* conn = new JSONRPC2Connection();
    bool running = false;

    LPythonServer() {
      running = true;
    }

    void send_diagnostics(std::string fpath) {
      std::map<std::string, std::string> diag_results = {
        {"start_index", "14"},
        {"end_index", "24"},
        {"hover_message", "Hello"},
      };

      conn->send_notification(
        diag_results
      );
    }

    void on_save(std::string fpath) {
      send_diagnostics(fpath);
    }

    void run(std::string path) {
      while (running) {
        on_save(path);
        running = false;
      }
    }
};

int main(int argc, char** argv) {
  LPythonServer* server = new LPythonServer(); 
  std::string path;
  if (argc <= 1) {
    path = "/home/ankita/main.py";
  } else {
    path = argv[1];
  }
  
  server->run(path);
}
