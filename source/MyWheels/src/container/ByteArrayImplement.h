#ifndef __MWL_BYTE_ARRAY_IMPLEMENT_H__
#define __MWL_BYTE_ARRAY_IMPLEMENT_H__

#include "inc/ByteArray.h"

namespace mwl {

    struct ByteArray::Implement  {
        Implement(int64_t initSize, uint8_t initVal);
        Implement(uint8_t *pData, int64_t dataSize, OwnerShip ownership);
        Implement(const Implement &rhs);
        ~Implement();
        uint8_t* _Begin();
        uint8_t* _End();
        uint8_t& _ElementAt(int64_t idx);
        void _Fill(uint8_t val);
        int64_t _Copy(const uint8_t *pData, int64_t dataSize);
        int64_t _Assign(const uint8_t *pData, int64_t dataSize);
        int64_t _Share(uint8_t *pData, int64_t dataLen);
        int64_t _Takeover(uint8_t *pData, int64_t dataLen);

        inline int64_t _Size() { return bufSize; }
        void _Reset(uint8_t *pData = NULL, int64_t dataSize = 0);

        uint8_t* pBuf;
        int64_t bufSize;
        bool shared;
    };

}

#endif // __MWL_BYTE_ARRAY_IMPLEMENT_H__
