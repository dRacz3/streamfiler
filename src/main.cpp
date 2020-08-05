#include <iostream>
#include "FileWriter.h"


#include <iostream>
#include <thread>
#include "TCPStreamReader.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    FileWriter fw("hello.txt");
    fw.write("Hello hello");
    TCPStreamReader reader(10, 1024, -1, fw);
    reader.listen(9213);
    reader.shutdown();
    return 0;
}
