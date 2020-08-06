#ifndef STREAMFILER_LOGGER_H
#define STREAMFILER_LOGGER_H

#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <utility>

#define LOGGER_NAME_LENGTH 14

class Logger {
public:
    enum LEVEL {
        DEBUG = 0,
        INFO,
        WARNING,
        ERROR,
    };

    explicit Logger(std::string loggerName = "ROOT") {
        if (loggerName.size() < LOGGER_NAME_LENGTH) {
            while (loggerName.size() < LOGGER_NAME_LENGTH) {
                loggerName += " ";
            }
        }
        m_name = loggerName;
    }

    void setLevel(LEVEL level) {
        m_level = level;
    }

    void debug(const std::string &message) const {
        if (m_level <= LEVEL::DEBUG) {
            std::cout << "[ DEBUG ]" << "[" << m_name << "] [" << getTimestamp() << "] " << message << std::endl;
        }
    }

    void info(const std::string &message) const {
        if (m_level <= LEVEL::INFO) {
            std::cout << "[ INFO  ]" << "[" << m_name << "] [" << getTimestamp() << "] " << message << std::endl;
        }
    }

    void warning(const std::string &message) const {
        if (m_level <= LEVEL::WARNING) {
            std::cout << "[WARNING]" << "[" << m_name << "] [" << getTimestamp() << "] " << message << std::endl;
        }
    }

    void error(const std::string &message) const {
        if (m_level <= LEVEL::ERROR) {
            std::cout << "[ ERROR ]" << "[" << m_name << "] [" << getTimestamp() << "] " << message << std::endl;
        }
    }


protected:

    std::string getTimestamp() const {
        auto timestamp = std::chrono::system_clock::now();

        std::time_t now_tt = std::chrono::system_clock::to_time_t(timestamp);
        std::tm tm = *std::localtime(&now_tt);

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d_%H:%M:%S");
        return ss.str();
    }

    std::string m_name;
    LEVEL m_level{LEVEL::INFO};
};

#endif //STREAMFILER_LOGGER_H
