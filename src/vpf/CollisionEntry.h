#pragma once
#include <cstdint>

namespace VPF {
#pragma pack(push, 1)
    struct CollisionEntry {
        uint32_t magic;
        uint8_t hashPath[16];
        uint16_t realPathLength;
        int64_t dataOffset;
        int64_t nextCollision;
        char* realPath;
    };
#pragma pack(pop)
}