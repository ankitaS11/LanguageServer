#include <string> 

#include "LPythonServer.hpp"

int main (int argc, char** argv) {
    std::string path;
    if (argc <= 1)
        path = "/home/ankita/Documents/git/LanguageServer/lpyth/__init__.py";
    else
        path = argv[1];
    LPythonServer().run(path);
}