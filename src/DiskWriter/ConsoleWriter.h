#ifndef STREAMFILER_CONSOLEWRITER_H
#define STREAMFILER_CONSOLEWRITER_H

#include <Utilities/Logger.h>

#include <string>

#include "ITextOutput.h"

class ConsoleWriter : public ITextOutput
{
public:
    explicit ConsoleWriter(const std::string& filename);
    ~ConsoleWriter() override;

    void write(const std::string &content) override;

    void close() override;

protected:
    Logger m_logger;
};


#endif //STREAMFILER_CONSOLEWRITER_H
