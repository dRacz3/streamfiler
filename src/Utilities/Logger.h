#ifndef STREAMFILER_LOGGER_H
#define STREAMFILER_LOGGER_H

#include <string>
#include <iostream>
#include <utility>

#define LOGGER_NAME_LENGTH 14

class Logger {
public:
    explicit Logger(std::string loggerName = "ROOT")  {
        if(loggerName.size() < LOGGER_NAME_LENGTH)
        {
            while(loggerName.size() < LOGGER_NAME_LENGTH)
            {
                loggerName+=" ";
            }
        }
        m_name = loggerName;
    }

    void info(const std::string &message) {
        std::cout << "[ INFO  ]" << "[" << m_name << "] " << message << std::endl;
    }

    void warning(const std::string &message) {
        std::cout << "[WARNING]" << "[" << m_name << "] " << message << std::endl;
    }

    void error(const std::string &message) {
        std::cout << "[ ERROR ]" << "[" << m_name << "] " << message << std::endl;
    }

protected:
    std::string m_name;
};

#endif //STREAMFILER_LOGGER_H
