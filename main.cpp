#include <iostream>
#include "command.h"
#include <unistd.h>

int main(int argc, char **argv) {
    int fd[2];

    if(pipe(fd) != 0) {
        std::cout << "Error while creating pipes" << std::endl;
    }

    command command_sender("ls", {argv[0], "/home/fmk/"}, fd, false);
    command_sender.exec_command();
    command command_receiver("tee", {argv[0]}, fd, true);
    command_receiver.exec_command();
    command_sender.wait_command();
    command_receiver.wait_command();


    return 0;
}