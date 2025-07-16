#include <iostream>
#include "include/Header.hpp"
#include "include/Constants.hpp"


int main() {
    std::string javaFile = "AquaEvent.java";
    tortoise_javah::Header gener(javaFile);
    int r = gener.generateJAVAH();
    if (r == tortoise_javah::EXIT_ERR) {
        std::cerr << "Failed!\n";
    }
    else if (r == tortoise_javah::EXIT_ERR_EXT) {
        std::cerr << "Incorrect file extension!\n";
    }
    else {
        std::cout << "File generated successfully!\n";
    }
    system("pause");
    return 0;
}