#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>
#include <iostream>
#include <ostream>


class Logger {
    private:
        std::string file_name;
        std::FILE* file;
    public:
        Logger() {
            this->file_name = "/home/ankita/Documents/git/logs.log";

            // std::FILE* file = std::fopen("/home/krshrimali/logs.log", "a");
            // this->file = file;
        }

        void log(std::string message) {
            std::FILE* file = std::fopen("/home/ankita/Documents/git/logs.log", "a");
            std::fwrite(message.data(), sizeof(message[0]), message.size(), file);
            this->exit(file);
        }

        void exit(std::FILE* file) {
            std::string end_line = "LOG EXITING...\n";
            std::fwrite(end_line.data(), sizeof(end_line[0]), end_line.size(), file);
            std::fclose(file);
        }
};
#endif