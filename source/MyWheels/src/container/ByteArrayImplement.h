#ifndef __MWL_BYTE_ARRAY_IMPLEMENT_H__
#define __MWL_BYTE_ARRAY_IMPLEMENT_H__

#include "inc/ByteArray.h"
#include "inc/SmartPointer.h"

namespace mwl {

    struct RawMemoryManager {
        uint8_t *pBuf;
        int32_t bufSize;
        bool shared;
        RawMemoryManager() {
            pBuf = NULL;
            bufSize = 0;
            shared = false;
        }

        ~RawMemoryManager() {
            Reset();
        }

        void Reset(uint8_t *pBuf_ = NULL, int32_t bufSize_ = 0, bool shared_ = false) {
            if (pBuf != pBuf_) {
                if (!shared) {
                    delete[] pBuf;
                }
                pBuf = pBuf_;
                bufSize = bufSize_;
                shared = shared_;
            }
        }
    };

    struct ByteArray::Implement  {
        Implement(int32_t initSize, uint8_t initVal);
        Implement(uint8_t *pData, int32_t dataSize, OwnerShip ownership);
        Implement(const Implement &rhs);
        ~Implement();
        const ByteArray::Iterator& _Begin();
        const ByteArray::ConstIterator& _CBegin();
        const ByteArray::Iterator& _End();
        const ByteArray::ConstIterator& _CEnd();
        uint8_t &_ElementAt(int32_t idx);
        void _Fill(uint8_t val);
        int32_t _Copy(const uint8_t *pSrc, int32_t srcLen, int32_t copyStartPos, int32_t copyLen);
        int32_t _Assign(const uint8_t *pSrc, int32_t srcLen, int32_t assignStartPos, int32_t assignLen);
        int32_t _Share(uint8_t *pSrc, int32_t srcLen, int32_t shareStartPos, int32_t shareLen);
        int32_t _Takeover(uint8_t *pData, int32_t dataLen);
        int32_t _Move(int32_t dst, int32_t src, int32_t dataSize);
        void _Reset();

        inline int32_t _Size() {
            return arrSize;
        }


        SharedPtr<RawMemoryManager> rawMem;
        uint8_t *pArray;
        int32_t arrStartPos;
        int32_t arrSize;
    };

    struct ByteArray::IteratorImplement {
        IteratorImplement() {
            pArrImpl = NULL;
            posInArr = 0;
        }
        ByteArray::Implement *pArrImpl;
        int32_t posInArr;
    }; 


}

#endif // __MWL_BYTE_ARRAY_IMPLEMENT_H__
