#ifndef __MWL_ARRAY_H__
#define __MWL_ARRAY_H__

#include "inc/BasicDefines.h"
#include "inc/ByteArray.h"

namespace mwl {

    template<typename ElementType>
    class Array {
    public:
        Array() {}
        Array(int32_t count, const ElementType &initVal = ElementType());
        Array(const ElementType *pData, int32_t count);
        Array(const Array<ElementType> &src);
        ~Array();

        Array<ElementType>& operator=(const Array<ElementType> &src);
        ElementType& operator[](int32_t idx);
        const ElementType& operator[](int32_t idx) const;
        ElementType& At(int32_t idx);
        const ElementType& At(int32_t idx) const;

        ElementType& Front();
        const ElementType& Front() const;
        ElementType& Back();
        const ElementType& Back() const;

        Array<ElementType>& Assign(const Array<ElementType> &src);
        Array<ElementType>& Assign(const ElementType* pSrc, int32_t count);
        Array<ElementType>& Append(const ElementType &element);
        Array<ElementType>& Append(const ElementType *pElements, int32_t count);

        int32_t Insert(int32_t insertPos, const ElementType &element);
        int32_t Insert(int32_t insertPos, const ElementType *pElements, int32_t count);
        int32_t Insert(int32_t insertPos, const Array<ElementType> &elements, int32_t count = -1);

        int32_t Remove(int32_t removePos, int32_t count = 1);

        const ElementType* Data(int32_t idx = 0) const;
        ElementType* Data(int32_t idx = 0);
        int32_t Increase(int32_t incSize, const ElementType &fillVal);
        int32_t Decrease(int32_t decSize, const ElementType &fillVal);
        int32_t Resize(int32_t newSize, const ElementType &fillVal);
        int32_t Size() const;
        void Clear();
        int32_t Reserve(int32_t capacity);
        int32_t Capacity() const;
        bool Empty() const;

        void Swap(Array<ElementType> &other);

    private:
        ByteArray _storage;
#ifdef __MWL_WIN__
        ElementType *_pElements;
#endif
    };
}

#include "ArrayImplement.h"

namespace std {
    template <typename ElementType>
    inline void swap(mwl::Array<ElementType> &a1, mwl::Array<ElementType> &a2) {
        a1.Swap(a2);
    }
}

#endif // __MWL_ARRAY_H__
