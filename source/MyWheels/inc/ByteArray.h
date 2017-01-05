#ifndef __MWL_BYTE_ARRAY_H__
#define __MWL_BYTE_ARRAY_H__

#include "BasicDefines.h"

namespace mwl {

    class MWL_API ByteArray {
    public:
        explicit ByteArray(int64_t initCapacity = 0);
        ByteArray(const ByteArray &rhs);
        ~ByteArray();

        ByteArray& operator=(const ByteArray & rhs);
        uint8_t operator[](int64_t idx) const;
        uint8_t& operator[](int64_t idx);

        uint8_t* Begin();
        const uint8_t* Begin() const;
        uint8_t* End();
        const uint8_t* End() const;
        uint8_t* RawData(int64_t startIdx);
        const uint8_t* RawData(int64_t startIdx) const;

        int64_t Reserve(int64_t newCapacity);
        int64_t Capacity() const;
        int64_t Increase(int32_t deltaSize);
        int64_t Decrease(int32_t deltaSize);
        int64_t Resize(int64_t newSize);
        int64_t Size() const;

        int32_t ShareConst(const uint8_t *pBuf, int64_t bufSize);
        int32_t ShareConst(const ByteArray &arr);
        int32_t ShareMutable(uint8_t *pBuf, int64_t bufSize);
        int32_t ShareMutable(ByteArray &arr);
        int32_t Copy(const uint8_t *pBuf, int64_t bufSize);
        int32_t Copy(const ByteArray &arr);
        int32_t Takeover(uint8_t *pBuf, int64_t bufSize);
        int32_t Takeover(ByteArray &arr);
        void Swap(ByteArray &arr);

        void Clear();

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_BYTE_ARRAY_H__
