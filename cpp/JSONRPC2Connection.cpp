#include "JSONRPC2Connection.hpp"

#include <iostream>


void JSONRPC2Connection::_send(
  std::map<std::string, std::map<std::string, std::string>> body_two
) {
  // ignore body_one for now
  // and just transfer body_two to std::cout
  
  for (auto it = body_two.begin(); it != body_two.end(); it++) {
    std::map<std::string, std::string> value = it->second;

    std::cout << "{";
    int count = 0;
    for (auto it_two = value.begin(); it_two != value.end(); it_two++) {
      std::string key = it_two->first;
      std::string value_two = it_two->second;
      if (key == "hover_message") {
        std::cout << '"' << key << '"' << ":" << '"' << value_two << '"';
      } else {
        std::cout << '"' << key << '"' << ":" << value_two;
      }
      count++;
      // when count is 3, means you reached last key, so no need to append , xD
      if (count <= 2) {
        std::cout << ",";
      }
    }

    std::cout << "}\n";
  }
}

// {"start_idx": "14", "end_idx": "21", "hover_message": "Hi"}
void JSONRPC2Connection::send_notification(const std::map<std::string, std::string>& params) {
  // std::map<std::string, std::string> body_one = {
  //   "jsonrpc": "2.0",
  //   "method": method
  // };

  std::map<std::string, std::map<std::string, std::string>> body_two = {
    {"params", params}
  };

  _send(body_two);
} 