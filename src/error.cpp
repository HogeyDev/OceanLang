#include "error.hpp"
#include <iostream>

void errorOut(std::string message, int code) {
    std::cout << message << std::endl;
    exit(code);
}
