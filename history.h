#ifndef CMPESHELL_HISTORY_H
#define CMPESHELL_HISTORY_H

#include <string>
#include <vector>

extern class history hist;

class history {
public:
    explicit history();
    void push(const std::string &input);
    void print();
private:
    int size = 0;
    int tail = 0;
    std::vector<std::string> hist;
};

#endif //CMPESHELL_HISTORY_H
