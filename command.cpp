#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "command.h"

command::command(std::string exec, std::vector<std::string> args, const int *fd, bool receiver) {
    this->exec = std::move(exec);
    this->args = new char *[args.size()];
    for (int i = 0; i < args.size(); ++i) {
        this->args[i] = new char[args[i].size()];
        this->args[i] = strcpy(this->args[i], args[i].c_str());
    }

    this->fd[0] = fd[0];
    this->fd[1] = fd[1];
}

void command::exec_command() {
    pid = fork();

    if (pid < -1) {
        std::cout << "An error occurred" << std::endl;
    } else if (pid == 0) {
        std::cout << "Command executing" << std::endl;
        if (fd[0] != 0) {
            if (receiver) {
                close(fd[1]);
                dup2(0, fd[0]);
            } else {
                close(fd[0]);
                dup2(1, fd[1]);
            }
        }

        execvp(exec.c_str(), args);
    } else {
    }
}

void command::wait_command() {
    wait(&pid);
}

