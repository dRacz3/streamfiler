#ifndef STREAMFILER_FILEWRITER_H
#define STREAMFILER_FILEWRITER_H


#include <string>

class FileWriter {
public:
    explicit FileWriter(std::string filename);

    void write(const std::string &content);

    void close();

protected:
    std::string filename;
};


#endif //STREAMFILER_FILEWRITER_H
