#ifndef JSONRPC2_CONNECTION_HPP
#define JSONRPC2_CONNECTION_HPP

#include <deque>
#include <string>
#include "utilities.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// this is basically self.conn
class JSONRPC2Connection {
  public:
    std::deque<std::string> _msg_buffer;
    Logger log;
    JSONRPC2Connection() {
      this->log = Logger(); 
    }
    json read_message();
    int _read_header_content_length(std::string);
    std::string read();
    json _receive();
    json _json_parse(std::string);
};

#endif