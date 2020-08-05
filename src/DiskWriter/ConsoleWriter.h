#ifndef STREAMFILER_CONSOLEWRITER_H
#define STREAMFILER_CONSOLEWRITER_H


#include <string>
#include <Utilities/Logger.h>
#include "IFileWriter.h"

class ConsoleWriter : public IFileWriter {
public:
    explicit ConsoleWriter(std::string filename);

    void write(const std::string &content) override;

    void close() override;

protected:
    std::string filename;
    Logger m_logger;
};


#endif //STREAMFILER_CONSOLEWRITER_H
