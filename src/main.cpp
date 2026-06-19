#include "Tests.hpp"
#include "UI.hpp"
#include <iostream>

int main() {
    try {
        RunAllTests();
    } catch (const std::exception& ex) {
        std::cerr << "Tests failed: " << ex.what() << std::endl;
        return 1;
    }

    RunUI();

    return 0;
}
