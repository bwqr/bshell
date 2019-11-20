#include <unistd.h>
#include <cstring>
#include "command.h"
#include "history.h"

command::command(const std::string &exec, const std::vector<std::string> &args) {
    this->exec = exec;
    this->argv = args;
}

void command::execute_command() {

    std::string command;

    if(exec == "listdir") {
        command = "ls";
    } else if(exec == "printfile") {
        command = "cat";
    } else if(exec == "currentpath") {
        command = "pwd";
    } else if(exec == "footprint") {
        hist.print();
        exit(0);
    }
    else {
        command = exec;
    }

    auto exec_argv = new char *[argv.size() + 1];

    for (int j = 0; j < argv.size(); ++j) {
        exec_argv[j] = new char[argv[j].size()];

        strcpy(exec_argv[j], argv[j].c_str());
    }

    exec_argv[argv.size()] = nullptr;

    execvp(command.c_str(), exec_argv);
}

void command::set_ifd(const int &ifd) {
    if(dup2(ifd, STDIN_FILENO) == -1) {
        std::cerr << "Error while dup2 on set_ifd" << std::endl;
    }
}

void command::set_ofd(const int &ofd) {
    if(dup2(ofd, STDOUT_FILENO) == -1) {
        std::cerr << "Error while dup2 on set_ifd" <<std::endl;
    }
}

