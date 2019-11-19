#include <iostream>
#include "command.h"
#include <unistd.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <queue>
#include <wait.h>

#define MAX_HIST

void print_line_prefix(const std::string &user);

std::string get_user();

std::string get_pwd();

std::vector<command> parse_commands(const std::string &input);

void save_hist(const std::string &input);

int main(int argc, char **argv) {
    int fd[2];

    if (pipe(fd) != 0) {
        std::cout << "Error while creating pipes" << std::endl;
    }

    std::string input;
    std::vector<command> commands;

    bool quit = false;

    while (!quit) {
        print_line_prefix(get_user());

        std::getline(std::cin, input);

        save_hist(input);

        if (input == "exit") {
            quit = true;
            continue;
        }

        commands = parse_commands(input);

        for (auto &command: commands) {
            command.exec_command();
        }

        // Wait for all processes
        waitpid(-1, NULL, 0);

        for (auto &command: commands) {
            command.close_pipes();
        }
    }

    return 0;
}

void print_line_prefix(const std::string &user) {
    std::cout << user << " >>> ";
}

std::string get_user() {
    return getenv("USER");
}

std::string get_pwd() {
    return getenv("PWD");
}

std::vector<command> parse_commands(const std::string &input) {
    //Split
    std::istringstream iss(input);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));

    std::vector<command> commands;

    struct pipe_options next_options = {};

    for (ulong i = 0; i < words.size(); ++i) {
        std::string command_string = words[i];

        std::vector<std::string> args;
        args.push_back(words[i]);

        struct pipe_options options = {};

        // Only one single pipe
        if(next_options.enabled) {
            options = next_options;
            next_options = {};
        }

        while (words.size() > (i + 1)) {

            i++;
            if (words[i] == "|") {

                int fd[2];

                if (pipe(fd) != 0) {
                    std::cout << "Error while creating pipe" << std::endl;
                }

                options.enabled = true;
                options.ifd = fd[0];
                options.ofd = fd[1];
                options.receiver = false;

                next_options.enabled = true;
                next_options.ifd = fd[0];
                next_options.ofd = fd[1];
                next_options.receiver = true;
                break;
            } else if (words[i] == ">") {
            } else {
                args.push_back(words[i]);
            }

        }


        commands.push_back(command(command_string, args, options));
    }

    return commands;
}

void save_hist(const std::string &input) {

}