#ifndef __MWL_BYTE_ARRAY_H__
#define __MWL_BYTE_ARRAY_H__

#include "BasicDefines.h"
#include "inc/SmartPointer.h"

namespace mwl {

    #define MWL_IT_COMMON_METHODS(ItClass) \
        ItClass(); \
        ItClass(const ItClass &src); \
        ~ItClass(); \
        ItClass& operator=(const ItClass &rhs); \
        ItClass& operator++(); \
        ItClass operator++(int32_t); \
        ItClass& operator--(); \
        ItClass operator--(int32_t); \
        ItClass& operator+=(int32_t step); \
        ItClass& operator-=(int32_t step); \
        ItClass operator+(int32_t step) const; \
        ItClass operator-(int32_t step) const; \
        bool operator==(const ItClass &rhs) const; \
        bool operator!=(const ItClass &rhs) const; \
        bool operator<(const ItClass &rhs) const; \
        bool operator>(const ItClass &rhs) const; \
        bool operator<=(const ItClass &rhs) const; \
        bool operator>=(const ItClass &rhs) const; \

    class MWL_API ByteArray {
    public:
        struct Iterator;
        struct ConstIterator;
        struct ReverseIterator;
        struct ConstReverseIterator;
        struct IteratorImplement;
        struct MWL_API Iterator {
            MWL_IT_COMMON_METHODS(Iterator);
            explicit Iterator(const ReverseIterator &src);
            uint8_t &operator*() const;
            uint8_t *operator->() const;
            IteratorImplement *m_pImpl;
        };

        struct MWL_API ConstIterator {
            MWL_IT_COMMON_METHODS(ConstIterator);
            ConstIterator(const ByteArray::Iterator &src);
            explicit ConstIterator(const ByteArray::ReverseIterator &src);
            explicit ConstIterator(const ByteArray::ConstReverseIterator &src);
            const uint8_t &operator*() const;
            const uint8_t *operator->() const;
            IteratorImplement *m_pImpl;
        };

        struct MWL_API ReverseIterator {
            MWL_IT_COMMON_METHODS(ReverseIterator);
            explicit ReverseIterator(const ByteArray::Iterator &src);
            uint8_t &operator*() const;
            uint8_t *operator->() const;
            IteratorImplement *m_pImpl;
        };

        struct MWL_API ConstReverseIterator {
            MWL_IT_COMMON_METHODS(ConstReverseIterator);
            ConstReverseIterator(const ByteArray::ReverseIterator &src);
            explicit ConstReverseIterator(const ByteArray::Iterator &src);
            explicit ConstReverseIterator(const ByteArray::ConstIterator &src);
            const uint8_t &operator*() const;
            const uint8_t *operator->() const;
            IteratorImplement *m_pImpl;
        };

        explicit ByteArray(int32_t initSize = 0, uint8_t initVal = 0);
        ByteArray(uint8_t *pData, int32_t dataSize, OwnerShip ownership = OWN_COPY);
        ByteArray(const uint8_t *pData, int32_t dataSize);
        ByteArray(const ByteArray &src);
        ~ByteArray();

        ByteArray &operator=(const ByteArray &rhs);
        uint8_t operator[](int32_t idx) const;
        uint8_t &operator[](int32_t idx);

        ByteArray::Iterator Begin();
        ByteArray::ConstIterator Begin() const;
        ByteArray::ConstIterator CBegin() const;

        ByteArray::ReverseIterator RBegin();
        ByteArray::ConstReverseIterator RBegin() const;
        ByteArray::ConstReverseIterator CRBegin() const;

        ByteArray::Iterator End();
        ByteArray::ConstIterator End() const;
        ByteArray::ConstIterator CEnd() const;

        ByteArray::ReverseIterator REnd();
        ByteArray::ConstReverseIterator REnd() const;
        ByteArray::ConstReverseIterator CREnd() const;

        uint8_t *Data(int32_t startIdx = 0);
        const uint8_t *Data(int32_t startIdx = 0) const;
        uint8_t At(int32_t idx) const;
        uint8_t &At(int32_t idx);

        int32_t Copy(const uint8_t *pSrc, int32_t copyLen);
        int32_t Copy(const ByteArray &src);
        int32_t Assign(const uint8_t *pSrc, int32_t assignLen);
        int32_t Assign(const ByteArray &src);
        int32_t Share(uint8_t *pSrc, int32_t srcLen);
        int32_t Share(const ByteArray &src);
        int32_t Takeover(uint8_t *pSrc, int32_t srcLen);
        int32_t Takeover(ByteArray &other);
        int32_t Move(int32_t dst, int32_t src, int32_t dataSize);
        void Swap(ByteArray &other);
        SharedPtr<ByteArray> Slice(int32_t start, int32_t end = -1);
        const SharedPtr<ByteArray> Slice(int32_t start, int32_t end = -1) const;
        void Fill(uint8_t val);
        void Clear();
        void Reset();

        int32_t Increase(int32_t incSize, uint8_t fillVal = 0);
        int32_t Decrease(int32_t decSize, uint8_t fillVal = 0);
        int32_t Resize(int32_t newSize, uint8_t fillVal = 0);
        int32_t Size() const;
        int32_t Reserve(int32_t newCapacity);
        int32_t Capacity() const;
        bool Empty() const;

        struct Implement;
    private:
        Implement *m_pImpl;
    };

}

namespace std {
    inline void swap(mwl::ByteArray &b1, mwl::ByteArray &b2) {
        b1.Swap(b2);
    }
}

#endif // __MWL_BYTE_ARRAY_H__
