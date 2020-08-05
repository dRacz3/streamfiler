#include <iostream>
#include "FileWriter.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    FileWriter fw("hello.txt");
    fw.write("Hello hello");
    return 0;
}
