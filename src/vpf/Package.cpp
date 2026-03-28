#include "Package.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include "Header.h"
#include <xxhash.h>
#include <vector>
#include "Hash128.h"
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
        if (!m_handle) {
            throw std::runtime_error("Package::has() cannot find element. m_handle == nullptr");
        }
        path = normalizePath(path);
        Hash128 pathHash = hashPath(path);

        const uint64_t slotCount = m_headerCache.hashTableSize / sizeof(PathTableEntry);
        uint64_t slot = pathHash.low % slotCount;

        uint64_t totalOffset = sizeof(Header) + sizeof(PathTableEntry) * slot;
        fseek(m_handle, (long)totalOffset, SEEK_SET);

        PathTableEntry pte;
        fread(&pte, sizeof(PathTableEntry), 1, m_handle);

        if (pte.dataOffset == 0) {
            return false;
        }

        fseek(m_handle, (long)pte.dataOffset, SEEK_SET);
        uint32_t magic = 0;
        fread(&magic, sizeof(uint32_t), 1, m_handle);

        if (magic == 0x76706666) {
            uint8_t hashBytes[16];
            memcpy(hashBytes,     &pathHash.low,  8);
            memcpy(hashBytes + 8, &pathHash.high, 8);
            return memcmp(hashBytes, pte.hashPath, 16) == 0;
        }

        if (magic == 0x76706663) {
            uint8_t hashBytes[16];
            memcpy(hashBytes,     &pathHash.low,  8);
            memcpy(hashBytes + 8, &pathHash.high, 8);

            uint64_t collisionOffset = pte.dataOffset;
            while (collisionOffset != 0xFFFFFFFFFFFFFFFFULL) {
                fseek(m_handle, (long)collisionOffset, SEEK_SET);

                CollisionEntryHeader ce;
                fread(&ce, sizeof(CollisionEntryHeader), 1, m_handle);

                if (memcmp(hashBytes, ce.hashPath, 16) == 0) {
                    return true;
                }

                collisionOffset = (uint64_t)ce.nextCollision;
            }
            return false;
        }

        return false;
    }

    void Package::open(std::string path) {
        m_handle = fopen(path.c_str(), "rb");
        if (!m_handle) {
            throw std::runtime_error("Cannot open package file: " + path);
        }
        Header header;
        fread(&header, sizeof(Header), 1, m_handle);
        if (header.magic != 0x56504600) {
            throw std::runtime_error("Package file magic does not match");
        }
        if (header.checksum != getChecksum()) {
            throw std::runtime_error("Package file checksum does not match");
        }
        m_headerCache = header;
        fseek(m_handle, sizeof(Header), SEEK_SET);
    }

    void Package::close() {
        if (m_handle) {
            fclose(m_handle);
            m_handle = nullptr;
        }
    }

    uint64_t Package::getChecksum() {
        if (!m_handle) {
            throw std::runtime_error("Cannot get checksum from package file");
        }

        fseek(m_handle, 0, SEEK_END);
        long fileSize = ftell(m_handle);
        fseek(m_handle, 0, SEEK_SET);

        std::vector<uint8_t> buf(fileSize);
        fread(buf.data(), 1, fileSize, m_handle);

        memset(buf.data() + 26, 0, 8);

        return XXH64(buf.data(), buf.size(), 0);
    }

    Hash128 Package::hashPath(std::string& path) {
        const uint8_t* data = reinterpret_cast<const uint8_t*>(path.data());
        size_t len = path.size();

        uint64_t lo = XXH64(data, len, 0);
        uint64_t hi = XXH64(data, len, 0x9E3779B185EBCA87ULL);

        return { lo, hi };
    }

    std::string Package::normalizePath(std::string path) {
        // Было: заменяет / на \ (неправильно!)
        // path.replace(path.begin(), path.end(), '/', '\\');

        for (char& c : path) {
            if (c == '\\') c = '/';
            c = (char)tolower((unsigned char)c);
        }
        return path;
    }

}
