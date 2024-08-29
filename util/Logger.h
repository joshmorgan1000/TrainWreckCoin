#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <semaphore>

// Static logger class
class Logger {
public:
    static std::counting_semaphore<1> log_writer_semaphore;

    static void log(const std::string& classId, const std::string& message) {
        log_writer_semaphore.acquire();
        std::cout << "[" << getTimestamp() << "] [" << classId.c_str() << "] " << message << std::endl;
        log_writer_semaphore.release();
    }

    static std::string getTimestamp() {
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X");
        return ss.str();
    }

};

std::counting_semaphore<1> Logger::log_writer_semaphore(1);

#endif //LOGGER_H
