#include "JSONRPC2Connection.hpp"

#include <string>
#include <iostream>
#include <iterator>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class EOFError : public std::exception {
  public:
    std::string what () {
      return "Reached end of file...";
    }
};

class JSONRPC2ProtocolError : public std::exception {
  public:
    std::string msg;
    JSONRPC2ProtocolError(std::string msg) {
        this->msg = msg;
    } 
    std::string what () {
      return msg;
    }
};

int JSONRPC2Connection::_read_header_content_length(std::string line) {
  if (line.size() < 2 || (line[line.size() - 1] == '\n' && line[line.size() - 2] == '\r'))  {
    throw JSONRPC2ProtocolError("Line endings must be \r\n");
  }

  if (line.substr(0, 16) == "Content-Length: ") {
    // Example -> Content-length: 100
    int length = std::stoi(line.substr(16, line.size() - 16));
    return length;
  }

  return -1;
}

std::string JSONRPC2Connection::read(int len) {
  std::cin >> std::noskipws;

  std::string body = "";
  int exit_seq = 0;
  for (int i = 0; i < len;) {
    int c = getchar();
    if (c == EOF) break;
    if (i == 0 && c != 123) continue;
    if (exit_seq == 0 && c == '\r') {
      ++exit_seq;
    } else if (exit_seq == 0 && c == '\n'){
      break;
    } else {
      body += c;
      i++;
    }
  }
  this->log.log("Readed body as: " + body + '\n');
  return body;
}

json JSONRPC2Connection::_json_parse(std::string body) {
  auto json_body = json::parse(body);
  return json_body;
}

json JSONRPC2Connection::_receive() {
  std::string line;
  std::getline(std::cin, line);
  if (line.size() == 0) {
    // EOFError
    this->log.log("Throwing EOF Error: ");
    throw(EOFError());
  }
  int length = this->_read_header_content_length(line);
  std::string body = this->read(length);
  return _json_parse(body);
}

void JSONRPC2Connection::_send(json& body) {
  std::string body_str = body.dump();
  int content_length = body_str.size();
  std::string response = "Content-Length: " + std::to_string(content_length) + "\r\n" +
    "Content-Type: application/vscode-jsonrpc; charset=utf8\r\n\r\n";
  std::cout << response;
  std::cout << body;
}

void JSONRPC2Connection::write_message(int rid, json& response) {
  json body = {
      {"jsonrpc", "2.0"},
      {"id", rid},
      {"result", response},
  };
  this->_send(body);
}

json JSONRPC2Connection::read_message() {
  json res_empty;
  try {
    json res = this->_receive();
    return res;
  } catch (EOFError) {
    return res_empty;
  }

  return res_empty;
}

void JSONRPC2Connection::send_notification(std::string method, json &params) {
  json body = {
            {"jsonrpc", "2.0"},
            {"method", method},
            {"params", params},
        };
        this->_send(body);
}