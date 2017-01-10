#ifndef __MWL_BYTE_ARRAY_H__
#define __MWL_BYTE_ARRAY_H__

#include "BasicDefines.h"

namespace mwl {

    enum OwnerShip {
        OWN_COPY,
        OWN_SHARE,
        OWN_TAKEOVER,
    };

    class MWL_API ByteArray {
    public:
        explicit ByteArray(int64_t initSize = 0, uint8_t initVal = 0);
        explicit ByteArray(uint8_t *pData, int64_t dataSize, OwnerShip ownership = OWN_COPY);
        ByteArray(const uint8_t *pData, int64_t dataSize);
        ByteArray(const ByteArray &rhs);
        ~ByteArray();

        ByteArray& operator=(const ByteArray & rhs);
        uint8_t operator[](int64_t idx) const;
        uint8_t& operator[](int64_t idx);
        uint8_t At(int64_t idx) const;
        uint8_t& At(int64_t idx);

        uint8_t* Begin();
        const uint8_t* Begin() const;
        uint8_t* End();
        const uint8_t* End() const;
        uint8_t* Data(int64_t startIdx = 0);
        const uint8_t* Data(int64_t startIdx = 0) const;

        bool Empty() const;
        int64_t Size() const;

        int64_t Copy(const uint8_t *pData, int64_t dataSize);
        int64_t Copy(const ByteArray &data, int64_t dataSize, int64_t startPos = 0);
        int64_t Assign(const uint8_t *pData, int64_t dataSize);
        int64_t Assign(const ByteArray &data, int64_t dataSize, int64_t startPos = 0);
        int64_t Share(uint8_t *pData, int64_t dataSize);
        int64_t Share(ByteArray &data, int64_t dataSize, int64_t startPos = 0);
        int64_t Takeover(uint8_t *pData, int64_t dataSize);
        int64_t Takeover(ByteArray &data);
        int64_t Move(int64_t dst, int64_t src, int64_t dataSize);

        void Swap(ByteArray &other);
        void Fill(uint8_t val);
        void Clear();

    private:
        struct Implement;
        Implement* m_pImpl;
    };

}

#endif // __MWL_BYTE_ARRAY_H__
