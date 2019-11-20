#include <iostream>
#include "command.h"
#include "history.h"
#include <unistd.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <queue>
#include <wait.h>
#include <fcntl.h>
#include <list>

struct pipe {
    int ifd = STDIN_FILENO;
    int ofd = STDOUT_FILENO;
};

class history hist;

void print_line_prefix(const std::string &user);

std::string get_user();

std::vector<command> parse_commands(const std::string &input, std::vector<struct pipe> *pipes);

void save_hist(const std::string &input);

void close_pipes(const std::vector<struct pipe> &pipes);

int main(int argc, char **argv) {
    std::string input;
    std::vector<command> commands;
    hist = history();

    bool quit = false;

    while (true) {
        print_line_prefix(get_user());

        if(!std::getline(std::cin, input) || input == "exit") {
            break;
        }

        save_hist(input);

        std::vector<struct pipe> pipes;
        commands = parse_commands(input, &pipes);

        for (ulong i = 0; i < commands.size(); i++) {
            auto &command = commands[i];

            int pid = fork();
            if (pid < 0) {
                std::cerr << "Error while forking" << std::endl;
            } else if (pid == 0) {

                if (i != 0) {
                    command::set_ifd(pipes[i - 1].ifd);
                }

                if (i < pipes.size()) {
                    command::set_ofd(pipes[i].ofd);
                }

                close_pipes(pipes);
                // No return
                command.execute_command();
            } else {
                //parent
            }
        }

        close_pipes(pipes);

        while (wait(nullptr) != -1);
    }

    return 0;
}

void print_line_prefix(const std::string &user) {
    std::cout << user << " >>> ";
}

std::string get_user() {
    return getenv("USER");
}

void close_pipes(const std::vector<struct pipe> &pipes) {
    for (auto &pipe: pipes) {
        if (pipe.ifd > STDIN_FILENO) {
            close(pipe.ifd);
        }
        if (pipe.ofd == STDIN_FILENO || pipe.ofd > STDOUT_FILENO) {
            close(pipe.ofd);
        }
    }
}

std::vector<command> parse_commands(const std::string &input, std::vector<struct pipe> *pipes) {
    //Split
    //TODO: error right here, words inside the quotes should not be divided
    std::istringstream iss(input);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));

    std::vector<command> commands;

    for (ulong i = 0; i < words.size(); ++i) {
        std::string command_string = words[i];

        std::vector<std::string> args;
        args.push_back(words[i]);

        while (words.size() > (i + 1)) {
            i++;

            if (words[i] == "|") {
                if (words.size() <= (i + 1)) {
                    std::cerr << "Invalid command" << std::endl;
                    goto error;
                }

                int fd[2];

                if (pipe(fd) == -1) {
                    std::cerr << "Error while creating pipe" << std::endl;
                    goto error;
                }

                pipes->push_back({fd[0], fd[1]});

                break;
            } else if (words[i] == ">") {
                i++;

                if (words.size() <= i) {
                    std::cerr << "No file supplied" << std::endl;

                    goto error;
                }

                int pfd = open(words[i].c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                               S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

                if (pfd == -1) {
                    std::cerr << "File cannot be created " << words[i] << std::endl;

                    goto error;
                }

                pipes->push_back({STDIN_FILENO, pfd});

                break;
            } else {
                args.push_back(words[i]);
            }

        }

        commands.emplace_back(command(command_string, args));
    }

    return commands;

    error:
    pipes->clear();
    return std::vector<command>();
}

void save_hist(const std::string &input) {
    hist.push(input);
}
