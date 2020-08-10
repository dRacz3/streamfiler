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
            std::cerr << "[WARNING]" << "[" << m_name << "] [" << getTimestamp() << "] " << message << std::endl;
        }
    }

    void error(const std::string &message) const {
        if (m_level <= LEVEL::ERROR) {
            std::cerr << "[ ERROR ]" << "[" << m_name << "] [" << getTimestamp() << "] " << message << std::endl;
        }
    }


protected:

    static std::string getTimestamp() {
        using namespace std::chrono;

        // get current time
        auto now = system_clock::now();

        // get number of milliseconds for the current second
        // (remainder after division into seconds)
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        // convert to std::time_t in order to convert to std::tm (broken time)
        auto timer = system_clock::to_time_t(now);

        // convert to broken time
        std::tm bt = *std::localtime(&timer);

        std::ostringstream oss;

        oss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S"); // HH:MM:SS
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return oss.str();
    }

    std::string m_name;
    LEVEL m_level{LEVEL::INFO};
};

#endif //STREAMFILER_LOGGER_H
