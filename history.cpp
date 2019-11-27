#include <iostream>
#include "history.h"

history::history(int n) {
    hist = std::vector<std::string>(n);
}

void history::push(const std::string &input) {
    hist[tail] = input;
    tail = (tail + 1) % (int) hist.size();
    size = std::min((int) hist.size(), size + 1);
}

void history::print() {
    for (int i = 0; i < size; ++i) {
        auto index = (tail - size + i);

        if(index < 0) {
            index += hist.size();
        }

        std::cout << (i + 1) << ". " << hist[index] << std::endl;
    }
}
