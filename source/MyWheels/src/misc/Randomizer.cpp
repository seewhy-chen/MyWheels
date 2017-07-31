#define NOMINMAX // disable macros 'min' and 'max'

#include "inc/Randomizer.h"

#include <time.h>

namespace mwl {

    union _FloatIntUnion {
        float fVal;
        uint32_t iVal;
    };

    static inline float _Bits2Float(uint32_t bits) {
        _FloatIntUnion fiu;
        fiu.iVal = bits;
        return fiu.fVal;
    }

    static inline uint32_t _NextLCG(uint32_t seed) {
        //  See "Numerical Recipes in C", 1992 page 284 for these constants
        //  For the LCG that sets the initial state from a seed
        return 1664525 * seed + 1013904223;
    }

    struct Randomizer::Implement {
        uint32_t _fK;
        uint32_t _fJ;

        int32_t _SetSeed(uint32_t seed) {
            if (0 == seed) {
                seed = static_cast<uint32_t>(time(NULL));
            }
            _fK = _NextLCG(seed);
            if (0 == _fK) {
                _fK = _NextLCG(_fK);
            }
            _fJ = _NextLCG(_fK);
            if (0 == _fJ) {
                _fJ = _NextLCG(_fJ);
            }
            return ERR_NONE;
        }

        uint32_t _NextU() {
            // magic numbers (30345 and 18000) are Constants for the multiply-with-carry steps
            _fK = 30345 * (_fK & 0xffff) + (_fK >> 16);
            _fJ = 18000 * (_fJ & 0xffff) + (_fJ >> 16);
            return (((_fK << 16) | (_fK >> 16)) + _fJ);
        }
    };

    Randomizer::Randomizer(uint32_t seed) : m_pImpl(new Implement()){
        m_pImpl->_SetSeed(seed);
    }

    Randomizer::~Randomizer() {
        delete m_pImpl;
    }

    int32_t Randomizer::SetSeed(uint32_t seed) {
        return m_pImpl->_SetSeed(seed);
    }

    int16_t Randomizer::NextS16() {
        return static_cast<int16_t>(m_pImpl->_NextU() >> 16);
    }

    uint16_t Randomizer::NextU16() {
        return m_pImpl->_NextU() >> 16;
    }

    int32_t Randomizer::NextS32() {
        return static_cast<int32_t>(m_pImpl->_NextU());
    }

    uint32_t Randomizer::NextU32() {
        return m_pImpl->_NextU();
    }

    int64_t Randomizer::NextS64() {
        return static_cast<int64_t>(NextU64());
    }

    uint64_t Randomizer::NextU64() {
        return (static_cast<uint64_t>(m_pImpl->_NextU()) << 32) | (m_pImpl->_NextU());
    }

    float Randomizer::NextFloat() {
        uint32_t floatint = 0x3f800000 | (m_pImpl->_NextU() >> 9);
        return _Bits2Float(floatint) - 1.0f;
    }

    int16_t Randomizer::NextS16InRange(int16_t min, int16_t max) {
        return static_cast<int16_t>(min + NextFloat() * (max - min));
    }

    uint16_t Randomizer::NextU16InRange(uint16_t min, uint16_t max) {
        return static_cast<uint16_t>(min + NextFloat() * (max - min));
    }

    int32_t Randomizer::NextS32InRange(uint32_t min, uint32_t max) {
        return static_cast<int32_t>(min + NextFloat() * (max - min));
    }

    uint32_t Randomizer::NextU32InRange(uint32_t min, uint32_t max) {
        return static_cast<uint32_t>(min + NextFloat() * (max - min));
    }

    int64_t Randomizer::NextS64InRange(int64_t min, int64_t max) {
        return static_cast<int64_t>(min + NextFloat() * (max - min));
    }

    uint64_t Randomizer::NextU64InRange(uint64_t min, uint64_t max) {
        return static_cast<uint64_t>(min + NextFloat() * (max - min));
    }

    float Randomizer::NextFloatInRange(float min, float max) {
        return min + NextFloat() * (max - min);
    }

} // namespace mwl
