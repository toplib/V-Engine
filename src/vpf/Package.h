#pragma once
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>

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

        bool has(std::string path);

    private:
        FILE* m_handle = nullptr;
        uint64_t getChecksum(Header header, std::vector<PathTableEntry> pathTableEntries, std::vector<FileEntryHeader> fileEntryHeaders, std::vector<CollisionEntry> collisionEntries);
    };
}
