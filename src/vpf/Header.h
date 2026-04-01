#pragma once
#include <cstdint>


namespace VPF {
#pragma pack(push, 1)
    struct Header {
        uint32_t magic;
        uint32_t version;
        uint16_t flags;
        uint64_t hashTableSize;
        uint64_t fileCount;
        uint64_t checksum;
        uint16_t compressionType;
        uint16_t encryptionType;
        uint8_t reserved[26];
    };
#pragma pack(pop)
}
