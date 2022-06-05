#include <iostream>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

    // a valid JSON text
    // std::string valid_text = R"(
    //     {"Key": "value", "DictKey": {"Key2": "value2"}}
    // )";

    // auto j3 = json::parse(valid_text);
    // std::cout << j3["DictKey"];

#include <map>
#include <string>

struct handle_functions
{
   void serve_initialize() { std::cout << "Initialized called \n"; }
   void serve_onSave() { std::cout << "Onsave function called"; }
};

typedef void (handle_functions::*handle)(void);
typedef std::map<std::string, handle> map_fun;

int main()
{

    map_fun handler;
    handler["initialize"] = &handle_functions::serve_initialize;
    handler["textDocument/didSave"] = &handle_functions::serve_onSave;

    std::string request_method = request["method"].get<std::string>();
    map_fun::iterator x = handler.find(request_method);

    if (x != handler.end()) {
        handle_functions h;
        (h.*(x->second))();
        std::cout << "Done";
    }
}


    // void handle(auto request) {
    //     map <std::string, > handler;


    // }
        // def noop(request: dict):
        //     return None

        // handler = {
        //     "initialize": self.serve_initialize,
        //     # "textDocument/didOpen": self.serve_onSave,
        //     "textDocument/didSave": self.serve_onSave,
        //     "textDocument/documentSymbol": self.serve_document_symbols,
        //     # "workspace/symbol": self.serve_workspace_symbol,
        //     # "textDocument/didChange": self.serve_onChange,
        //     # "initialized": noop,
        //     # "$/cancelRequest": noop,
        //     # "$/setTrace": noop,
        //     # "shutdown": noop,
        //     # "exit": self.serve_exit,
        // }.get(request["method"], self.serve_default)

        // if "id" not in request:
        //     try:
        //         handler(request)
        //     except:
        //         print("error got")
        //     return

        // try:
        //     resp = handler(request)
        // except JSONRPC2Error as e:
        //     self.conn.write_error(
        //         request["id"], code=e.code, message=e.message, data=e.data
        //     )
        // except Exception as e:
        //     self.conn.write_error(
        //         request["id"],
        //         code=-32603,
        //         message=str(e),
        //         data={
        //             "traceback": traceback.format_exc(),
        //         },
        //     )
        // else:
        //     self.conn.write_response(request["id"], resp)
