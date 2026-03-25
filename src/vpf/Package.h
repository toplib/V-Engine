#pragma once
#include <cstdio>
#include <string>

namespace VPF {
    class Package {
    public:
        Package();
        Package(std::string path);
        ~Package();

        void open(std::string path);
        void close();

        bool has(std::string path);

    private:
        FILE* m_handle = nullptr;
    };
}
