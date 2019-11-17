#ifndef CMPESHELL_COMMAND_H
#define CMPESHELL_COMMAND_H

#include <iostream>
#include <vector>
#include <string>

class command {
public:

    int fd[2] = {0, 0};
    bool receiver;
    std::string exec;
    pid_t pid;

    explicit command(std::string exec, std::vector<std::string> args, const int *fd, bool receiver);

    void exec_command();

    void wait_command();

private:
    char **args;
};


#endif //CMPESHELL_COMMAND_H
