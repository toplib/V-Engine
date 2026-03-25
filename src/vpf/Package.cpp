#include "Package.h"

#include <stdexcept>
#include "Header.h"
#include <xxhash.hpp>

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
        int currentOffset = sizeof(Header);
        fseek(m_handle, currentOffset, SEEK_SET);

        for (int i = 0; i < header.fileCount; i++) {
            fread(&pathTableEntries, sizeof(PathTableEntry), 1, m_handle);
            currentOffset += sizeof(PathTableEntry);
            fseek(m_handle, currentOffset, SEEK_SET);
        }
        printf("%d entries \n", pathTableEntries.size());
        for (int i = 0; i < pathTableEntries.size(); i++) {
            printf("HashPath: 0x");
            for (int j = 0; j < 16; j++) {
                printf("%02X \n", pathTableEntries[i].hashPath[j]);
            }
            printf("DataOffset: 0x%16llX \n", pathTableEntries[i].dataOffset);
        }
    }

    void Package::close() {
        if (m_handle) {
            fclose(m_handle);
            m_handle = nullptr;
        }
    }
}
