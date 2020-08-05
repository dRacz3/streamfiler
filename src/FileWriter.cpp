#include "FileWriter.h"

#include <utility>
#include <iostream>

FileWriter::FileWriter(std::string filename) : filename(std::move(filename)) {}

void FileWriter::close() {

}

void FileWriter::write(const std::string &content) {
    std::cout << "[" << filename << "] -> " << content << std::endl;
}
