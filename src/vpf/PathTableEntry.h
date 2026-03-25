#pragma once
#pragma pack(push, 1)
#include <cstdint>

namespace VPF {

    struct PathTableEntry {
        uint8_t hashPath[16];
        int64_t dataOffset;
    };
}