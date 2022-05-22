#ifndef JSON_RPC
#define JSON_RPC

#include <string>
#include <map>

class JSONRPC2Connection {
  public:
    void _send(std::map<std::string, std::map<std::string, std::string>> body);
    void send_notification(
      const std::map<std::string, std::string>& params
    );
};

#endif