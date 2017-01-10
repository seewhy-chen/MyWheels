#include "ByteArrayImplement.h"

#include <algorithm> // for std::min

namespace mwl {

    static uint8_t s_dummy = 0;
    ByteArray::Implement::Implement(int64_t initSize, uint8_t initVal) {
        pBuf = NULL;
        bufSize = 0;
        shared = false;
        if (initSize > 0) {
            bufSize = initSize;
            pBuf = new uint8_t[static_cast<size_t>(bufSize)];
            memset(pBuf, initVal, static_cast<size_t>(bufSize));
        }
    }

    ByteArray::Implement::Implement(uint8_t *pData, int64_t dataSize, OwnerShip ownership) {
        pBuf = NULL;
        bufSize = 0;
        shared = false;
        if (pData && dataSize > 0) {
            switch (ownership) {
            case OWN_TAKEOVER:
            case OWN_SHARE: {
                    pBuf = pData;
                    shared = (OWN_SHARE == ownership);
                }
                break;
            case OWN_COPY: {
                    pBuf = new uint8_t[static_cast<size_t>(dataSize)];
                    memcpy(pBuf, pData, static_cast<size_t>(dataSize));
                }
                break;
            }
            bufSize = dataSize;
        }
    }

    ByteArray::Implement::Implement(const ByteArray::Implement &rhs) {
        bufSize = 0;
        if (rhs.bufSize > 0 && rhs.pBuf) {
            memcpy(pBuf, rhs.pBuf, static_cast<size_t>(rhs.bufSize));
            bufSize = rhs.bufSize;
        }
    }

    ByteArray::Implement::~Implement() {
        _Reset();
    }

    uint8_t *ByteArray::Implement::_Begin() {
        return bufSize > 0 ? pBuf : NULL;
    }

    uint8_t *ByteArray::Implement::_End() {
        return bufSize > 0 ? pBuf + bufSize : NULL;
    }

    uint8_t &ByteArray::Implement::_ElementAt(int64_t idx) {
        if (0 <= idx && idx < bufSize) {
            return pBuf[idx];
        } else {
            return s_dummy;
        }
    }

    void ByteArray::Implement::_Fill(uint8_t val) {
        memset(pBuf, val, static_cast<size_t>(bufSize));
    }

    int64_t ByteArray::Implement::_Copy(const uint8_t *pData, int64_t dataSize) {
        if (pData && dataSize > 0) {
            int64_t dataCanCopy = std::min<int64_t>(dataSize, bufSize);
            memcpy(pBuf, pData, static_cast<size_t>(dataCanCopy));
            return dataCanCopy;
        } else {
            return 0;
        }
    }

    int64_t ByteArray::Implement::_Assign(const uint8_t *pData, int64_t dataSize) {
        if (shared) {
            pBuf = NULL;
            bufSize = 0;
            shared = false;
        }

        if (pData && dataSize > 0) {
            if (dataSize > bufSize) {
                _Reset();
                pBuf = new uint8_t[static_cast<size_t>(dataSize)];
            }
            memcpy(pBuf, pData, static_cast<size_t>(dataSize));
            bufSize = dataSize;
        }
        return bufSize;
    }

    int64_t ByteArray::Implement::_Share(uint8_t *pData, int64_t dataSize) {
        _Reset(pData, dataSize);
        shared = true;
        return bufSize;
    }

    int64_t ByteArray::Implement::_Takeover(uint8_t *pData, int64_t dataSize) {
        _Reset(pData, dataSize);
        shared = false;
        return bufSize;
    }

    void ByteArray::Implement::_Reset(uint8_t *pData, int64_t dataSize) {
        if (pData != pBuf) {
            if (!shared) {
                delete[] pBuf;
            }
            pBuf = pData;
        }
        bufSize = dataSize;
    }

}
