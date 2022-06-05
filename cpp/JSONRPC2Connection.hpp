#ifndef JSONRPC2_CONNECTION_HPP
#define JSONRPC2_CONNECTION_HPP

#include <deque>
#include <string>
#include "utilities.hpp"

// this is basically self.conn
class JSONRPC2Connection {
  public:
    std::deque<std::string> _msg_buffer;
    Logger log;
    JSONRPC2Connection() {
      this->log = Logger(); 
    }
    std::string read_message();
    int _read_header_content_length(std::string);
    std::string read();
    auto _receive();
    auto _json_parse(std::string);
};

#endif