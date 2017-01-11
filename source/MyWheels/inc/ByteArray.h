#ifndef __MWL_BYTE_ARRAY_H__
#define __MWL_BYTE_ARRAY_H__

#include "BasicDefines.h"
#include "inc/SmartPointer.h"

namespace mwl {

    class MWL_API ByteArray {
    public:
        explicit ByteArray(int32_t initSize = 0, uint8_t initVal = 0);
        explicit ByteArray(uint8_t *pData, int32_t dataSize, OwnerShip ownership = OWN_COPY);
        ByteArray(const uint8_t *pData, int32_t dataSize);
        ByteArray(const ByteArray &rhs);
        ~ByteArray();

        ByteArray &operator=(const ByteArray &rhs);
        uint8_t operator[](int32_t idx) const;
        uint8_t &operator[](int32_t idx);

        uint8_t At(int32_t idx) const;
        uint8_t &At(int32_t idx);
        uint8_t *Begin();
        const uint8_t *Begin() const;
        uint8_t *End();
        const uint8_t *End() const;
        uint8_t *Data(int32_t startIdx = 0);
        const uint8_t *Data(int32_t startIdx = 0) const;

        int32_t Copy(const uint8_t *pSrc, int32_t copyLen);
        int32_t Copy(const ByteArray &src);
        int32_t Assign(const uint8_t *pSrc, int32_t assignLen);
        int32_t Assign(const ByteArray &src);
        int32_t Share(uint8_t *pSrc, int32_t srcLen);
        int32_t Share(ByteArray &src);
        int32_t Takeover(uint8_t *pSrc, int32_t srcLen);
        int32_t Takeover(ByteArray &other);
        int32_t Move(int32_t dst, int32_t src, int32_t dataSize);
        void Swap(ByteArray &other);
        SharedPtr<ByteArray> Slice(int32_t start, int32_t end = -1);
        const SharedPtr<ByteArray> Slice(int32_t start, int32_t end = -1) const;
        void Fill(uint8_t val);
        void Clear();
        void Reset();

        bool Empty() const;
        int32_t Size() const;

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_BYTE_ARRAY_H__
