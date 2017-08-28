#ifndef __MWL_ARRAY_IMPLEMENT_H__
#define __MWL_ARRAY_IMPLEMENT_H__

#ifdef __MWL_LINUX__
#include <stdlib.h> // on Linux, placement new will call exit()
#endif
#include <new> // for placement new


#ifdef __MWL_WIN__
#define _SET_ELE_PTR \
        do { \
            _pElements = reinterpret_cast<ElementType*>(_storage.Data()); \
        } while (0);
#else
#define _SET_ELE_PTR
#endif

namespace mwl {
    template <typename ElementType>
    inline void _Create(uint8_t *pRawMem, const ElementType &initVal, int32_t count) {
        for (int32_t i = 0; i < count; ++i) {
            new (pRawMem + i * sizeof(ElementType)) ElementType(initVal);
        }
    }

    template <typename ElementType>
    inline void _Create(uint8_t *pRawMem, const ElementType *pVals, int32_t count) {
        for (int32_t i = 0; i < count; ++i) {
            new (pRawMem + i * sizeof(ElementType)) ElementType(pVals[i]);
        }
    }

    template <typename ElementType>
    inline void _Release(uint8_t *pRawMem, int32_t count) {
        for (int32_t i = 0; i < count; ++i) {
            reinterpret_cast<ElementType *>(pRawMem + i * sizeof(ElementType))->~ElementType();
        }
    }

    template<typename ElementType>
    Array<ElementType>::Array(int32_t count, const ElementType &initVal) {
        if (count > 0) {
            _storage.Resize(sizeof(ElementType) * count);
            _Create(_storage.Data(), initVal, count);
        }
        _SET_ELE_PTR;
    }

    template<typename ElementType>
    Array<ElementType>::Array(const ElementType *pData, int32_t count) {
        if (count > 0) {
            _storage.Resize(count * sizeof(ElementType));
            _Create(_storage.Data(), pData, count);
        }
        _SET_ELE_PTR;
    }

    template<typename ElementType>
    Array<ElementType>::Array(const Array<ElementType> &src)
        : _storage(src._storage.Size()) {
        if (!_storage.Empty()) {
            _Create(_storage.Data(), src.Data(), src.Size());
        }
        _SET_ELE_PTR;
    }

    template<typename ElementType>
    Array<ElementType>::~Array() {
        Clear();
    }

    template<typename ElementType>
    Array<ElementType>& Array<ElementType>::operator=(const Array<ElementType> &src) {
        return Assign(src);
    }

    template<typename ElementType>
    ElementType& Array<ElementType>::operator[](int32_t idx) {
        return At(idx);
    }

    template<typename ElementType>
    const ElementType& Array<ElementType>::operator[](int32_t idx) const {
        return At(idx);
    }

    template<typename ElementType>
    ElementType& Array<ElementType>::At(int32_t idx) {
        if (idx < 0) {
            idx += Size();
        }
        if (idx < 0 || idx >= Size()) {
            static ElementType s_dummy;
            return s_dummy;
        }
        return *(reinterpret_cast<ElementType*>(_storage.Data(idx * sizeof(ElementType))));
    }

    template<typename ElementType>
    const ElementType& Array<ElementType>::At(int32_t idx) const {
        return const_cast<Array<ElementType>*>(this)->At(idx);
    }

    template<typename ElementType>
    ElementType& Array<ElementType>::Front() {
        return At(0);
    }

    template<typename ElementType>
    const ElementType& Array<ElementType>::Front() const {
        return At(0);
    }

    template<typename ElementType>
    ElementType& Array<ElementType>::Back() {
        return At(Size() - 1);
    }

    template<typename ElementType>
    const ElementType& Array<ElementType>::Back() const {
        return At(Size() - 1);
    }

    template<typename ElementType>
    Array<ElementType>& Array<ElementType>::Assign(const Array<ElementType> &src) {
        if (this != &src) {
            Clear();
            if (!src.Empty() && this != &src) {
                _storage.Resize(src._storage.Size());
                _Create(_storage.Data(), src.Data(), src.Size());
            }
            _SET_ELE_PTR;
        }
        return *this;
    }

    template<typename ElementType>
    Array<ElementType>& Array<ElementType>::Assign(const ElementType* pSrc, int32_t count) {
        if (this->_storage.Data() != pSrc) {
            Clear();
            if (count > 0) {
                _storage.Resize(count * sizeof(ElementType));
                _Create(_storage.Data(), pSrc, count);
            }
            _SET_ELE_PTR;
        }
        return *this;
    }

    template<typename ElementType>
    Array<ElementType>& Array<ElementType>::Append(const ElementType &element) {
        return Append(&element, 1);
    }

    template<typename ElementType>
    Array<ElementType>& Array<ElementType>::Append(const ElementType *pElements, int32_t count) {
        if (count > 0) {
            _storage.Increase(sizeof(ElementType) * count);
            _Create(_storage.Data((Size() - count) * sizeof(ElementType)), pElements, count);
            _SET_ELE_PTR;
        }
        return *this;
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Insert(int32_t insertPos, const ElementType &element) {
        return Insert(insertPos, &element, 1);
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Insert(int32_t insertPos, const Array<ElementType> &elements, int32_t count) {
        if (count < 0) {
            count = elements.Size();
        }
        return Insert(insertPos, elements.Data(), count);
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Insert(int32_t insertPos, const ElementType *pElements, int32_t count) {
        if (count <= 0) {
            return 0;
        }
        if (insertPos < 0) {
            insertPos += Size();
        }
        if (insertPos < 0) {
            insertPos = 0;
        }
        if (insertPos > Size()) {
            insertPos = Size();
        }

        int32_t origSize = Size();
        _storage.Increase(sizeof(ElementType) * count);
        if (insertPos < origSize) {
            memmove(Data(insertPos + count), Data(insertPos), (origSize - insertPos) * sizeof(ElementType));
        }
        _Create(_storage.Data(insertPos * sizeof(ElementType)), pElements, count);
        _SET_ELE_PTR;
        return count;
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Remove(int32_t removePos, int32_t count) {
        if (count <= 0) {
            return 0;
        }
        if (removePos < 0) {
            removePos += Size();
        }
        if (removePos < 0 || removePos >= Size()) {
            return 0;
        }

        int32_t rmvCnt = Size() - removePos;
        if (rmvCnt > count) {
            rmvCnt = count;
        }
        _Release<ElementType>(reinterpret_cast<uint8_t*>(Data(removePos)), rmvCnt);
        memmove(Data(removePos), Data(removePos + rmvCnt), (Size() - removePos - rmvCnt) * sizeof(ElementType));
        _storage.Decrease(sizeof(ElementType)*rmvCnt);
        _SET_ELE_PTR;
        return rmvCnt;
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Size() const {
        return _storage.Size() / sizeof(ElementType);
    }

    template<typename ElementType>
    bool Array<ElementType>::Empty() const {
        return _storage.Empty();
    }

    template<typename ElementType>
    ElementType* Array<ElementType>::Data(int32_t idx) {
        if (idx < 0) {
            idx += Size();
        }
        if (idx < 0 || idx >= Size()) {
            return nullptr;
        }
        return reinterpret_cast<ElementType*>(_storage.Data(idx * sizeof(ElementType)));
    }

    template<typename ElementType>
    const ElementType* Array<ElementType>::Data(int32_t idx) const {
        return const_cast<Array<ElementType>*>(this)->Data(idx);
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Increase(int32_t incSize, const ElementType &fillVal) {
        return Resize(Size() + incSize, fillVal);
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Decrease(int32_t decSize, const ElementType &fillVal) {
        return Resize(Size() - decSize, fillVal);
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Resize(int32_t newSize, const ElementType &fillVal) {
        int32_t origSize = Size();
        if (newSize > origSize) {
            _storage.Resize(newSize * sizeof(ElementType));
            _Create(_storage.Data(origSize * sizeof(ElementType)), fillVal, newSize - origSize);
        } else if (newSize < origSize) {
            _Release<ElementType>(_storage.Data(origSize * sizeof(ElementType)), origSize - newSize);
            _storage.Resize(newSize * sizeof(ElementType));
        }
        _SET_ELE_PTR;
        return Size();
    }

    template<typename ElementType>
    void Array<ElementType>::Clear() {
        if (!_storage.Empty()) {
            _Release<ElementType>(_storage.Data(), Size());
            _storage.Clear();
            _SET_ELE_PTR;
        }
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Reserve(int32_t capacity) {
        if (capacity > 0) {
            _storage.Reserve(capacity * sizeof(ElementType));
            _SET_ELE_PTR;
        }
        return Capacity();
    }

    template<typename ElementType>
    int32_t Array<ElementType>::Capacity() const {
        return _storage.Capacity() / sizeof(ElementType);
    }

    template<typename ElementType>
    void Array<ElementType>::Swap(Array<ElementType> &other) {
        _storage.Swap(other._storage);
    }
}

#endif // __MWL_ARRAY_IMPLEMENT_H__
