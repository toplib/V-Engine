#pragma once

#include <string>
#include <cstdint>

namespace VPF {
    struct Hash128 {
        uint64_t low;
        uint64_t high;

        bool operator==(const Hash128& object) const {
            return low == object.low && high == object.high;
        }
    };
}
