#ifndef STREAMFILER_ITEXTOUTPUT_H
#define STREAMFILER_ITEXTOUTPUT_H

#include <string>

class ITextOutput
{
public:
    virtual void write(const std::string &content) = 0;

    virtual void close() = 0;
};

#endif  // STREAMFILER_ITEXTOUTPUT_H
