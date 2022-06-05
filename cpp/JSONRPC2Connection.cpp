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
    // Content-length: 100
    int length = std::stoi(line.substr(16, line.size() - 16));
    return length;
  }

  return -1;
}

std::string JSONRPC2Connection::read() {
  // int i = 0;
  // std::string output = "";
  // while (i <= length) {
  //   char c;
  //   std::cin >> c;
  //   output.push_back(c);
  //   i++;
  // }
  // return output;
  // Do not skip whitespace, more configuration may also be needed.
  this->log.log("Reached reading body...");
  std::cin >> std::noskipws;

  // Copy all data from cin, using iterators.
  std::istream_iterator<char> begin(std::cin);
  std::istream_iterator<char> end;
  std::string json(begin, end);
  this->log.log("Got json as: " + json);

  // Use the JSON data somehow.
  return json;
}

auto JSONRPC2Connection::_json_parse(std::string body) {
  auto json_body = json::parse(body); 
  auto str = json_body["jsonrpc"].get<std::string>();
  this->log.log("json:" + str);
  return json_body;
}

auto JSONRPC2Connection::_receive() {
  std::string line;
  std::getline(std::cin, line);
  this->log.log("Got line: " + line);
  if (line.size() == 0) {
    // EOFError
    this->log.log("Throwing EOF Error: ");
    throw(EOFError());
  }

  this->log.log("Now reading length");
  int length = this->_read_header_content_length(line);
  this->log.log("Got length as: " + std::to_string(length));

  this->log.log("Getting each line till we get r and n characters\n");
  // while (line != "\r\n") {
  //   this->log.log("getting line: " + line);
  //   std::getline(std::cin, line);
  // }

  this->log.log("Reading body...\n");
  std::string body = this->read();
  // this->log.log("Got body: " + body);
  _json_parse(body);
  return body;
}

std::string JSONRPC2Connection::read_message() {
  // if want is None is true 
  this->log.log("LOG READING MESSAGE...\n");
  // if (this->_msg_buffer.size() != 0) {
  //   this->log.log("BUFFER NOT EMPTY\n");
  //   std::string output = this->_msg_buffer.front();
  //   this->_msg_buffer.pop_front();
  //   this->log.log("We received output: " + output);
  //   return output;
  // }
  std::string res = "";
  try {
    // this->log.log("Now receiving...\n");
    res = this->_receive();
    // this->log.log("We received output: " + res);
    return res;
  } catch (EOFError) {
    // this->log.log("End of file...\n");
    return res;
  }

  // this->log.log("Returning empty msg from read_message\n");
  return res;
}