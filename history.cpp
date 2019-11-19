#include <iostream>
#include "history.h"
#include "defs.h"

history::history() {
    hist = std::vector<std::string>(MAX_HIST);
}

void history::push(const std::string &input) {
    hist[tail] = input;
    tail = (tail + 1) % MAX_HIST;
    size = std::min(MAX_HIST, size + 1);
}

void history::print() {
    for (int i = 0; i < size; ++i) {
        std::cout << hist[(tail - size + i) % MAX_HIST] << std::endl;
    }
}
