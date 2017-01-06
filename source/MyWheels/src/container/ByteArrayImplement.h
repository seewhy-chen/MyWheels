#ifndef __MWL_BYTE_ARRAY_IMPLEMENT_H__
#define __MWL_BYTE_ARRAY_IMPLEMENT_H__

#include "inc/ByteArray.h"

namespace mwl {

    struct ImplementInternal {
        ImplementInternal(int64_t initCapacity, uint8_t initVal);
        ImplementInternal(const ImplementInternal &rhs);
        ~ImplementInternal();
        ImplementInternal& operator=(const ImplementInternal &rhs);
        uint8_t* _Begin();
        uint8_t* _End();
        uint8_t& _ElementAt(int64_t idx);
        int32_t _Assign(const uint8_t *pData, int64_t dataSize);
        int32_t _Takeover(ByteArray &arr);
        int32_t _Takeover(uint8_t *pData, int64_t dataLen);
        int64_t _Reserve(int64_t newCap);
        int64_t _Resize(int64_t newSize, uint8_t initVal);
        int64_t _Shrink();
        void _Clear();
        void _Reset();

        inline int64_t _Size() { return bufSize; }
        inline int64_t _Capacity() { return bufCap; }

        uint8_t* pBuf;
        int64_t bufCap;
        int64_t bufSize;
        bool isOwner;
        bool isMutable;
    };

    struct ByteArray::Implement {
        Implement(int64_t initCapacity, uint8_t initVal) : impl(new ImplementInternal(initCapacity, initVal)) {}
        Implement(const Implement& rhs) : impl(new ImplementInternal(*rhs.impl.get())) {}
        SharedPtr<ImplementInternal> impl;
    };
}

#endif // __MWL_BYTE_ARRAY_IMPLEMENT_H__
