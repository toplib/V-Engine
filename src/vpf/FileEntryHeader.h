#pragma once
#include <cstdint>

namespace VPF {
#pragma pack(push, 1)
    struct FileEntryHeader {
        uint32_t magic;
        uint64_t fileSize;
        uint64_t checksum;
    };
#pragma pack(pop)
}
