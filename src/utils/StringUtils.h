#pragma once
#include <string>

namespace Utils {

    inline void trim(std::string& s)
    {
        const auto first = s.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos) {
            s.clear();
            return;
        }
        const auto last = s.find_last_not_of(" \t\n\r\f\v");
        s = s.substr(first, last - first + 1);
    }

    inline std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::istringstream stream(str);
        std::string token;

        while (std::getline(stream, token, delimiter)) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }

        return tokens;
    }
}