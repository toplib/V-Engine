#pragma once
#include <cstdint>
#include <string>

namespace VPF {
#pragma pack(push, 1)
    struct CollisionEntryHeader {
        uint32_t magic;
        uint8_t hashPath[16];
        uint16_t realPathLength;
        uint64_t dataOffset;
        uint64_t nextCollision;
    };
#pragma pack(pop)
    struct CollisionEntry {
        CollisionEntryHeader header;
        std::string realPath;
    };
}