#include "ByteArrayImplement.h"

namespace mwl {

    static uint8_t s_dummy = 0;
    ImplementInternal::ImplementInternal(int64_t initSize, uint8_t initVal) {
        pBuf = NULL;
        isOwner = true;
        bufSize = bufCap = 0;
        if (initSize > 0) {
            bufCap = bufSize = initSize;
            pBuf = new uint8_t[static_cast<size_t>(bufCap)];
            memset(pBuf, initVal, static_cast<size_t>(bufCap));
        }
    }

    ImplementInternal::ImplementInternal(const ImplementInternal& rhs) {
        isOwner = true;
        bufSize = 0;
        _Reserve(rhs.bufCap);
        if (rhs.bufSize > 0 && rhs.pBuf) {
            memcpy(pBuf, rhs.pBuf, static_cast<size_t>(rhs.bufSize));
            bufSize = rhs.bufSize;
        }
    }

    ImplementInternal::~ImplementInternal() {
    }

    ImplementInternal& ImplementInternal::operator=(const ImplementInternal &rhs) {
        if (this != &rhs) {
            _Assign(rhs.pBuf, rhs.bufSize);
        }
        return *this;
    }

    uint8_t* ImplementInternal::_Begin() {
        return bufSize > 0 ? pBuf : NULL;
    }

    uint8_t* ImplementInternal::_End() {
        return bufSize > 0 ? pBuf + bufSize: NULL;
    }

    uint8_t& ImplementInternal::_ElementAt(int64_t idx) {
        if (0 <= idx && idx < bufSize) {
            return pBuf[idx];
        } else {
            return s_dummy;
        }
    }

    int32_t ImplementInternal::_Assign(const uint8_t *pData, int64_t dataSize) {
        if (pData != pBuf) {
            _Resize(dataSize, 0);
            if (pData && dataSize > 0) {
                memcpy(pBuf, pData, static_cast<size_t>(dataSize));
            } 
        }
        return ERR_NONE;
    }

    int32_t ImplementInternal::_Takeover(uint8_t *pData, int64_t dataSize) {
        //if (pData && dataSize > 0) {
        //    pBuf.reset(new SharedPtr<uint8_t>(pData));
        //    bufSize = bufCap = dataSize;
        //} else {
        //    _Reset();
        //}
        return ERR_NONE;
    }

    int32_t ImplementInternal::_Takeover(ByteArray &arr) {
        return ERR_NONE;
    }

    int64_t ImplementInternal::_Reserve(int64_t newCap) {
        if (newCap > bufCap) {
            uint8_t* pEnlargedBuf = new uint8_t[static_cast<size_t>(newCap)];
            memcpy(pEnlargedBuf, pBuf, static_cast<size_t>(bufSize));
            pBuf = pEnlargedBuf;
            bufCap = newCap;
        }
        return bufCap;
    }

    int64_t ImplementInternal::_Resize(int64_t newSize, uint8_t initVal) {
        _Reserve(newSize);
        if (newSize > bufSize) {
            memset(pBuf + bufSize, initVal, static_cast<size_t>(newSize - bufSize));
        }
        bufSize = newSize;
        return bufSize;
    }

    int64_t ImplementInternal::_Shrink() {
        if (bufCap > bufSize) {
            if (!bufSize) {
                _Reset();
            } else {
                uint8_t *pShrankBuf = new uint8_t[static_cast<size_t>(bufSize)];
                memcpy(pShrankBuf, pBuf, static_cast<size_t>(bufSize));
                pBuf = pShrankBuf;
                bufCap = bufSize;
            }
        }
        return bufCap;
    }

    void ImplementInternal::_Clear() {
        bufSize = 0;
    }

    void ImplementInternal::_Reset() {
        delete[] pBuf;
        pBuf = NULL;
        bufSize = bufCap = 0;
    }
}
