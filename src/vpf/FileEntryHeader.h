#pragma once
#include <cstdint>

namespace VPF {
    struct FileEntryHeader {
        uint32_t magic;
        uint64_t fileSize;
        uint64_t checksum;
    };
}