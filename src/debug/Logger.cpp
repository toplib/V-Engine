#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>

namespace Debug {

    static std::mutex g_mutex;
    static constexpr int moduleLen = 10;   // ширина имени модуля
    static constexpr const char* utcOffset = ""; // при желании: "+03:00"

    Logger::Logger(std::string loggerClass)
        : m_loggerClass(std::move(loggerClass)) {}

    Logger::~Logger() = default;

    void Logger::printDebug(const std::string& message) {
#ifndef NDEBUG
        write('D', std::cout, message);
#endif
    }

    void Logger::printInfo(const std::string& message) {
        write('I', std::cout, message);
    }

    void Logger::printWarning(const std::string& message) {
        write('W', std::cout, message);
    }

    void Logger::printError(const std::string& message) {
        write('E', std::cerr, message);
    }

    void Logger::write(char level,
                       std::ostream& out,
                       const std::string& message)
    {
        using namespace std::chrono;

        std::stringstream ss;

        ss << '[' << level << ']';

        auto now = system_clock::now();
        auto sec = system_clock::to_time_t(now);
        auto ms  = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &sec);
#else
        localtime_r(&sec, &tm);
#endif

        ss << ' ' << std::put_time(&tm, "%Y/%m/%d %T");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        ss << utcOffset;

        ss << " ["
           << std::setfill(' ')
           << std::setw(moduleLen)
           << m_loggerClass
           << "] ";

        ss << message;

        std::lock_guard<std::mutex> lock(g_mutex);
        out << ss.str() << std::endl;
    }

}