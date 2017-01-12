#include "InternalCommon.h"
#include "ByteArrayImplement.h"

namespace mwl {

    static uint8_t s_dummy = 0;
    ByteArray::Implement::Implement(int32_t initSize, uint8_t initVal)
        : rawMem(new RawMemoryManager()), pArray(nullptr), arrStartPos(0), arrSize(0) {
        if (initSize > 0) {
            rawMem->pBuf = new uint8_t[arrSize];
            rawMem->bufSize = initSize;
            memset(rawMem->pBuf, initVal, arrSize);
            arrSize = initSize;
            pArray = rawMem->pBuf + arrStartPos;
        }
        begin.m_pImpl->pArrImpl = this;
        begin.m_pImpl->posInArr = 0;
        end.m_pImpl->pArrImpl = this;
        end.m_pImpl->posInArr = arrSize;
    }

    ByteArray::Implement::Implement(uint8_t *pData, int32_t dataSize, OwnerShip ownership)
        : rawMem(new RawMemoryManager()), pArray(nullptr), arrStartPos(0), arrSize(0) {
        if (pData && dataSize > 0) {
            switch (ownership) {
            case OWN_TAKEOVER:
            case OWN_SHARE: {
                    rawMem->pBuf = pData;
                    rawMem->bufSize = dataSize;
                    rawMem->shared = (OWN_SHARE == ownership);
                }
                break;
            case OWN_COPY: {
                    rawMem->pBuf = new uint8_t[dataSize];
                    rawMem->bufSize = dataSize;
                    memcpy(rawMem->pBuf, pData, dataSize);
                }
                break;
            }
            pArray = rawMem->pBuf;
            arrSize = dataSize;
        }
        begin.m_pImpl->pArrImpl = this;
        begin.m_pImpl->posInArr = 0;
        end.m_pImpl->pArrImpl = this;
        end.m_pImpl->posInArr = arrSize;
    }

    ByteArray::Implement::Implement(const ByteArray::Implement &rhs)
        : rawMem(new RawMemoryManager()), pArray(nullptr), arrStartPos(0), arrSize(0) {
        if (rhs.arrSize > 0 && rhs.rawMem && rhs.rawMem->pBuf) {
            rawMem->pBuf = new uint8_t[rhs.arrSize];
            rawMem->bufSize = rhs.arrSize;
            memcpy(rawMem->pBuf, rhs.pArray, rhs.arrSize);
            pArray = rawMem->pBuf;
            arrSize = rhs.arrSize;
        }
        begin.m_pImpl->pArrImpl = this;
        begin.m_pImpl->posInArr = 0;
        end.m_pImpl->pArrImpl = this;
        end.m_pImpl->posInArr = arrSize;
    }

    ByteArray::Implement::~Implement() {
    }

    ByteArray::Iterator ByteArray::Implement::_Begin() {
        return begin;
    }

    ByteArray::Iterator ByteArray::Implement::_End() {
        return end;
    }

    uint8_t &ByteArray::Implement::_ElementAt(int32_t idx) {
        if (0 <= idx && idx < arrSize) {
            return pArray[idx];
        } else {
            return s_dummy;
        }
    }

    void ByteArray::Implement::_Fill(uint8_t val) {
        if (pArray && arrSize > 0) {
            memset(pArray, val, arrSize);
        }
    }

    int32_t ByteArray::Implement::_Copy(const uint8_t *pSrc, int32_t srcLen, int32_t copyStartPos, int32_t copyLen) {
        if (!pSrc || srcLen <= 0) {
            pSrc = nullptr;
            srcLen = 0;
        }
        if (copyStartPos < 0 || copyStartPos >= srcLen || copyLen <= 0) {
            return 0;
        }
        int32_t dataCanCopy = std::min(arrSize, std::min(srcLen - copyStartPos, copyLen));
        if (dataCanCopy > 0) {
            if (pSrc != rawMem->pBuf) {
                memcpy(pArray, pSrc + copyStartPos, dataCanCopy);
            } else {
                memmove(pArray, pSrc + copyStartPos, dataCanCopy);
            }
            return dataCanCopy;
        }
        return dataCanCopy;
    }

    int32_t ByteArray::Implement::_Assign(const uint8_t *pSrc, int32_t srcLen, int32_t assignStartPos, int32_t assignLen) {
        if (!pSrc || srcLen <= 0) {
            pSrc = nullptr;
            srcLen = 0;
        }
        if (assignStartPos < 0 || assignStartPos >= srcLen || assignLen < 0) {
            return ERR_INVAL_PARAM;
        }

        int32_t dataCanAssign = std::min(srcLen - assignStartPos, assignLen);
        if (pSrc == rawMem->pBuf) {
            arrStartPos = assignStartPos;
        } else {
            rawMem.reset(new RawMemoryManager());
            arrSize = 0;
            arrStartPos = 0;
            if (dataCanAssign > 0) {
                rawMem->pBuf = new uint8_t[dataCanAssign];
                rawMem->bufSize = dataCanAssign;
                memcpy(rawMem->pBuf, pSrc + assignStartPos, dataCanAssign);
            }
        }
        pArray = rawMem->pBuf + arrStartPos;
        arrSize = dataCanAssign;
        end.m_pImpl->posInArr = arrSize;
        return arrSize;
    }

    int32_t ByteArray::Implement::_Share(uint8_t *pSrc, int32_t srcLen, int32_t shareStartPos, int32_t shareLen) {
        if (!pSrc || srcLen <= 0) {
            pSrc = nullptr;
            shareLen = 0;
        }
        if (shareStartPos < 0 || shareStartPos >= srcLen || shareLen < 0) {
            return ERR_INVAL_PARAM;
        }
        int32_t dataCanShare = std::min(srcLen - shareStartPos, shareLen);
        rawMem->Reset(pSrc, srcLen, true);
        arrStartPos = shareStartPos;
        pArray = rawMem->pBuf + arrStartPos;
        arrSize = dataCanShare;
        end.m_pImpl->posInArr = arrSize;
        return arrSize;
    }

    int32_t ByteArray::Implement::_Takeover(uint8_t *pData, int32_t dataSize) {
        if (!pData || dataSize <= 0) {
            pData = nullptr;
            dataSize = 0;
        }
        rawMem.reset(new RawMemoryManager());
        rawMem->pBuf = pData;
        rawMem->bufSize = dataSize;
        pArray = rawMem->pBuf;
        arrStartPos = 0;
        arrSize = dataSize;
        end.m_pImpl->posInArr = arrSize;
        return arrSize;
    }

    int32_t ByteArray::Implement::_Move(int32_t dst, int32_t src, int32_t moveLen) {
        if (src < 0 || src >= arrSize || dst < 0 || dst >= arrSize) {
            return ERR_INVAL_PARAM;
        }
        if (src == dst) {
            return 0;
        }
        int32_t dataCanMove = std::min(arrSize - dst, std::min(arrSize - src, moveLen));
        memmove(pArray + dst, pArray + src, dataCanMove);
        return dataCanMove;
    }

    void ByteArray::Implement::_Reset() {
        rawMem.reset(new RawMemoryManager());
        pArray = nullptr;
        arrStartPos = 0;
        arrSize = 0;
        end.m_pImpl->posInArr = arrSize;
    }

}
