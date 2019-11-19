#ifndef CMPESHELL_COMMAND_H
#define CMPESHELL_COMMAND_H

#include <iostream>
#include <vector>
#include <string>
#include "pipe_options.h"

class command {
public:
    explicit command(std::string exec, std::vector<std::string> args, const struct pipe_options &pipe_options);

    void exec_command();

    void close_pipes();

    void wait_command();

private:
    std::string exec;
    pid_t child_pid;
    struct pipe_options pipe_options;
    char **args;
};


#endif //CMPESHELL_COMMAND_H
