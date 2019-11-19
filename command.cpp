#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "command.h"
#include "pipe_options.h"

command::command(std::string exec, std::vector<std::string> args, const struct pipe_options &pipe_options) {
    this->exec = std::move(exec);
    this->args = new char *[args.size() + 1];
    for (int i = 0; i < args.size(); ++i) {
        this->args[i] = new char[args[i].size()];
        this->args[i] = strcpy(this->args[i], args[i].c_str());
    }
    //null terminated args
    this->args[args.size()] = nullptr;

    this->pipe_options = pipe_options;
}

void command::exec_command() {
    child_pid = fork();

    if (child_pid < -1) {
        std::cout << "An error occurred" << std::endl;
    } else if (child_pid == 0) {
        if (pipe_options.enabled) {
            if (pipe_options.receiver) {
//                close(pipe_options.ofd);
//                std::cerr << exec << " " << "receiving to " << pipe_options.ifd << std::endl;
                if(dup2(pipe_options.ifd, STDIN_FILENO) == -1) {
                    std::cerr << "Dup3 error on receiver " << errno;
                }
            } else {
//                close(pipe_options.ifd);
//                std::cerr << exec << " " << "sending to " << pipe_options.ofd << std::endl;
                if(dup2(pipe_options.ofd, STDOUT_FILENO) == -1) {
                    std::cerr << "Dup3 error on sender" << errno;
                }
            }
        }

        execvp(exec.c_str(), args);
    } else {
    }
}

void command::wait_command() {
    wait(&child_pid);
}

void command::close_pipes() {
    if(pipe_options.enabled) {
        close(pipe_options.ifd);
        close(pipe_options.ofd);
    }
}

