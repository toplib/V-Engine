#pragma once
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include <map>

#include "Hash128.h"
#include "Header.h"
#include "CollisionEntry.h"
#include "FileEntryHeader.h"
#include "PathTableEntry.h"

namespace VPF {
    class Package {
    public:
        Package();
        Package(std::string path);
        ~Package();

        void open(std::string path);
        void close();

        bool has(std::string& path);
        std::vector<uint8_t> read(std::string& path);

    private:
        FILE* m_handle = nullptr;
        Header m_headerCache;
        uint64_t getChecksum();
        static Hash128 hashPath(const std::string& path);
        static std::string normalizePath(const std::string& path);
        uint64_t getFileOffset(const std::string& path);
    };
}
