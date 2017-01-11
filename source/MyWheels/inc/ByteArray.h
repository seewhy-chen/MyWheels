#ifndef __MWL_BYTE_ARRAY_H__
#define __MWL_BYTE_ARRAY_H__

#include "BasicDefines.h"
#include "inc/SmartPointer.h"

namespace mwl {

    class MWL_API ByteArray {
    public:
        struct IteratorImplement;
        struct MWL_API Iterator {
            Iterator();
            Iterator(const Iterator &src);
            ~Iterator();
            Iterator& operator=(const Iterator &rhs);

            uint8_t& operator*() const;
            uint8_t* operator->() const;

            Iterator& operator++();
            Iterator operator++(int32_t);

            Iterator& operator--();
            Iterator operator--(int32_t);

            Iterator& operator+=(int32_t step);
            Iterator& operator-=(int32_t step);

            Iterator operator+(int32_t step) const;
            Iterator operator-(int32_t step) const;

            bool operator<(const Iterator &rhs) const;
            bool operator>(const Iterator &rhs) const;
            bool operator<=(const Iterator &rhs) const;
            bool operator>=(const Iterator &rhs) const;
            bool operator==(const Iterator &rhs) const;
            bool operator!=(const Iterator &rhs) const;

            IteratorImplement *m_pImpl;
        };

        struct MWL_API ConstIterator {
            ConstIterator();
            ConstIterator(const ConstIterator &src);
            ConstIterator(const ByteArray::Iterator &src);
            ~ConstIterator();
            ConstIterator& operator=(const ConstIterator &rhs);

            uint8_t& operator*() const;
            uint8_t* operator->() const;

            ConstIterator& operator++();
            ConstIterator operator++(int32_t);

            ConstIterator& operator--();
            ConstIterator operator--(int32_t);

            ConstIterator& operator+=(int32_t step);
            ConstIterator& operator-=(int32_t step);

            ConstIterator operator+(int32_t step) const;
            ConstIterator operator-(int32_t step) const;

            bool operator<(const ConstIterator &rhs) const;
            bool operator>(const ConstIterator &rhs) const;
            bool operator<=(const ConstIterator &rhs) const;
            bool operator>=(const ConstIterator &rhs) const;
            bool operator==(const ConstIterator &rhs) const;
            bool operator!=(const ConstIterator &rhs) const;

            IteratorImplement *m_pImpl;
        };

        explicit ByteArray(int32_t initSize = 0, uint8_t initVal = 0);
        explicit ByteArray(uint8_t *pData, int32_t dataSize, OwnerShip ownership = OWN_COPY);
        ByteArray(const uint8_t *pData, int32_t dataSize);
        ByteArray(const ByteArray &rhs);
        ~ByteArray();

        ByteArray &operator=(const ByteArray &rhs);
        uint8_t operator[](int32_t idx) const;
        uint8_t &operator[](int32_t idx);

        const ByteArray::Iterator& Begin();
        const ByteArray::ConstIterator& Begin() const;
        const ByteArray::ConstIterator& CBegin() const;
        const ByteArray::Iterator& End();
        const ByteArray::ConstIterator& End() const;
        const ByteArray::ConstIterator& CEnd() const;

        uint8_t *Data(int32_t startIdx = 0);
        const uint8_t *Data(int32_t startIdx = 0) const;
        uint8_t At(int32_t idx) const;
        uint8_t &At(int32_t idx);

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

        struct Implement;
    private:
        Implement *m_pImpl;
    };

}

#endif // __MWL_BYTE_ARRAY_H__
