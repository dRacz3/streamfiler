#ifndef STREAMFILER_IFILEWRITER_H
#define STREAMFILER_IFILEWRITER_H

class IFileWriter {
public:
    virtual void write(const std::string &content) = 0;

    virtual void close() = 0;
};

#endif //STREAMFILER_IFILEWRITER_H
