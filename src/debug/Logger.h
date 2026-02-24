#pragma once
#include <string>
#include "LoggingType.h"

namespace Debug {
    class Logger {
    public:
        explicit Logger(std::string loggerClass);
        ~Logger();

        void printDebug(const std::string& message);
        void printInfo(const std::string& message);
        void printWarning(const std::string& message);
        void printError(const std::string& message);

    private:
        void write(char level,
                   std::ostream& out,
                   const std::string& message);

        std::string m_loggerClass;
    };
}
