#include "ConsoleWriter.h"

#include <utility>
#include <iostream>

ConsoleWriter::ConsoleWriter(std::string filename) : filename(std::move(filename)), m_logger(filename + "_WRITER") {}

void ConsoleWriter::close() {

}

void ConsoleWriter::write(const std::string &content) {
    std::cout << "[" << filename << "] -> " << content << std::endl;
}
