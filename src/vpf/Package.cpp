#include "Package.h"

#include <stdexcept>
#include "Header.h"
#include <xxhash.h>
#include <vector>

#include "CollisionEntry.h"
#include "FileEntryHeader.h"
#include "PathTableEntry.h"

namespace VPF {
    Package::Package() = default;
    Package::~Package() {
        close();
    }

    Package::Package(std::string path) {
        open(path);
    }


    bool Package::has(std::string path) {
        return false;
    }

    void Package::open(std::string path) {
        m_handle = fopen(path.c_str(), "rb");
        if (!m_handle) {
            throw std::runtime_error("Cannot open package file: " + path);
        }
        Header header;
        fread(&header, sizeof(Header), 1, m_handle);

        printf("Magic: 0x%08X \n", header.magic);
        printf("Version: 0x%08X \n", header.version);
        printf("Flags: 0x%04X \n", header.flags);
        printf("HashTableSize: %llu bytes \n", (unsigned long long) header.hashTableSize);
        printf("FileCount: %llu \n", (unsigned long long) header.fileCount);
        printf("Checksum: 0x%16llX \n", header.checksum);
        printf("CompressionType: 0x%02X \n", header.compressionType);
        printf("EncryptionType: 0x%02X \n", header.encryptionType);

        std::vector<PathTableEntry> pathTableEntries;
        const uint64_t slotCount = header.hashTableSize / sizeof(PathTableEntry);

        for (uint64_t i = 0; i < slotCount; i++) {
            PathTableEntry entry;
            if (fread(&entry, sizeof(PathTableEntry), 1, m_handle) != 1) {
                throw std::runtime_error("Failed to read path table entry " + std::to_string(i));
            }
            pathTableEntries.push_back(entry);
        }
        printf("%d entries \n", pathTableEntries.size());
        for (int i = 0; i < pathTableEntries.size(); i++) {
            printf("HashPath: 0x");
            for (int j = 0; j < 16; j++) {
                printf("%02X", pathTableEntries[i].hashPath[j]);
            }
            printf("\nDataOffset: 0x%16llX \n", pathTableEntries[i].dataOffset);
        }
    }

    void Package::close() {
        if (m_handle) {
            fclose(m_handle);
            m_handle = nullptr;
        }
    }

    uint64_t Package::getChecksum(
            Header header,
            std::vector<PathTableEntry> pathTableEntries,
            std::vector<FileEntryHeader> fileEntryHeaders,
            std::vector<CollisionEntry> collisionEntries
        ) {
        header.checksum = 0;
        XXH128_hash_t headerChecksum = XXH3_128bits(&header, sizeof(header));
        XXH128_hash_t fileEntryHeadersChecksum;
        for (int i = 0; i < fileEntryHeaders.size(); i++){
            fileEntryHeadersChecksum = XXH3_128bits(&fileEntryHeaders[i], sizeof(fileEntryHeaders[i]) + fileEntryHeaders[i].fileSize);
        }
        return headerChecksum.high64 ^ headerChecksum.low64;
    }


}
