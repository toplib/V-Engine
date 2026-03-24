#pragma once
#include <cstdint>

namespace VPF {
    struct Header {
        uint32_t magic;
        uint32_t version;
        uint16_t flags;
        int64_t dataOffset;
        uint64_t hashTableSize;
        uint64_t checksum;
        uint8_t reserved[35];
    };
}
