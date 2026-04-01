#pragma once
#include <cstdint>

namespace VPF {

#pragma pack(push, 1)
    struct PathTableEntry {
        uint8_t hashPath[16];
        int64_t dataOffset;
    };
#pragma pack(pop)

}