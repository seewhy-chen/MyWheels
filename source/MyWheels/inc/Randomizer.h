#ifndef __MWL__RANDOMIZER_H__
#define __MWL__RANDOMIZER_H__

#include "inc/BasicDefines.h"

namespace mwl {

    class MWL_API Randomizer : private NonCopyable {
    public:
        Randomizer(uint32_t seed = 0);
        ~Randomizer();
        int32_t SetSeed(uint32_t seed);

        int16_t NextS16();
        uint16_t NextU16();
        int32_t NextS32();
        uint32_t NextU32();
        int64_t NextS64();
        uint64_t NextU64();
        float NextFloat();

        int16_t NextS16InRange(int16_t min, int16_t max);
        uint16_t NextU16InRange(uint16_t min, uint16_t max);
        int32_t NextS32InRange(uint32_t min, uint32_t max);
        uint32_t NextU32InRange(uint32_t min, uint32_t max);
        int64_t NextS64InRange(int64_t min, int64_t max);
        uint64_t NextU64InRange(uint64_t min, uint64_t max);
        float NextFloatInRange(float min, float max);

    private:
        struct Implement;
        Implement *m_pImpl;
    };
}

#endif // __MWL__RANDOMIZER_H__
