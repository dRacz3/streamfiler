#ifndef STREAMFILER_LOGGER_H
#define STREAMFILER_LOGGER_H

#include <string>
#include <iostream>
#include <utility>

class Logger {
public:
    explicit Logger(std::string name = "ROOT") : m_name(std::move(name)) {

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
