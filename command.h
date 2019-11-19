#ifndef CMPESHELL_COMMAND_H
#define CMPESHELL_COMMAND_H

#include <iostream>
#include <vector>
#include <string>

class command {
public:
    explicit command(const std::string &exec, const std::vector<std::string> &args);

    void execute_command();

    static void set_ifd(const int &ifd);

    static void set_ofd(const int &ofd);
private:
    std::string exec;
    std::vector<std::string> argv;
};


#endif //CMPESHELL_COMMAND_H
