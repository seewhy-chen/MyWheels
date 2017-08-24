#include "InternalCommon.h"

#include "inc/ByteArray.h"
#include "ByteArrayImplement.h"

namespace mwl {

    ByteArray::ByteArray(int32_t initSize, uint8_t initVal)
        : m_pImpl(new Implement(initSize, initVal)) {}

    ByteArray::ByteArray(uint8_t *pData, int32_t dataSize, OwnerShip ownership)
        : m_pImpl(new Implement(pData, dataSize, ownership)) {}

    ByteArray::ByteArray(const uint8_t *pData, int32_t dataSize)
        : m_pImpl(new Implement(const_cast<uint8_t *>(pData), dataSize, OWN_COPY)) {}

    ByteArray::ByteArray(const ByteArray &rhs)
        : m_pImpl(new Implement(const_cast<uint8_t *>(rhs.Data()), rhs.Size(), OWN_COPY)) {}

    ByteArray::~ByteArray() {
        delete m_pImpl;
    }

    ByteArray &ByteArray::operator=(const ByteArray &rhs) {
        if (this != &rhs) {
            m_pImpl->_Assign(rhs.m_pImpl->rawMem->pBuf, rhs.m_pImpl->rawMem->bufSize,
                             rhs.m_pImpl->arrStartPos, rhs.m_pImpl->arrSize);
        }
        return *this;
    }

    uint8_t ByteArray::operator[](int32_t idx) const {
        return m_pImpl->_ElementAt(idx);
    }

    uint8_t &ByteArray::operator[](int32_t idx) {
        return m_pImpl->_ElementAt(idx);
    }

    uint8_t ByteArray::At(int32_t idx) const {
        return m_pImpl->_ElementAt(idx);
    }

    uint8_t &ByteArray::At(int32_t idx) {
        return m_pImpl->_ElementAt(idx);
    }

    ByteArray::Iterator ByteArray::Begin() {
        return m_pImpl->_Begin();
    }

    ByteArray::ConstIterator ByteArray::Begin() const {
        return ByteArray::ConstIterator(m_pImpl->_Begin());
    }

    ByteArray::ConstIterator ByteArray::CBegin() const {
        return ByteArray::ConstIterator(m_pImpl->_Begin());
    }

    ByteArray::ReverseIterator ByteArray::RBegin() {
        return ByteArray::ReverseIterator(m_pImpl->_End());
    }

    ByteArray::ConstReverseIterator ByteArray::RBegin() const {
        return ByteArray::ConstReverseIterator(m_pImpl->_End());
    }

    ByteArray::ConstReverseIterator ByteArray::CRBegin() const {
        return ByteArray::ConstReverseIterator(m_pImpl->_End());
    }

    ByteArray::Iterator ByteArray::End() {
        return m_pImpl->_End();
    }

    ByteArray::ConstIterator ByteArray::End() const {
        return CEnd();
    }

    ByteArray::ConstIterator ByteArray::CEnd() const {
        return ByteArray::ConstIterator(m_pImpl->_End());
    }

    ByteArray::ReverseIterator ByteArray::REnd() {
        return ByteArray::ReverseIterator(m_pImpl->_Begin());
    }

    ByteArray::ConstReverseIterator ByteArray::REnd() const {
        return ByteArray::ConstReverseIterator(m_pImpl->_Begin());
    }

    ByteArray::ConstReverseIterator ByteArray::CREnd() const {
        return ByteArray::ConstReverseIterator(m_pImpl->_Begin());
    }

    uint8_t *ByteArray::Data(int32_t startIdx) {
        return &m_pImpl->_ElementAt(startIdx);
    }

    const uint8_t *ByteArray::Data(int32_t startIdx) const {
        return &m_pImpl->_ElementAt(startIdx);
    }

    int32_t ByteArray::Increase(int32_t incSize, uint8_t fillVal) {
        return m_pImpl->_Resize(m_pImpl->_Size() + incSize, fillVal);
    }

    int32_t ByteArray::Decrease(int32_t decSize, uint8_t fillVal) {
        return m_pImpl->_Resize(m_pImpl->_Size() - decSize, fillVal);
    }

    int32_t ByteArray::Resize(int32_t newSize, uint8_t fillVal) {
        return m_pImpl->_Resize(newSize, fillVal);
    }

    int32_t ByteArray::Size() const {
        return m_pImpl->_Size();
    }

    int32_t ByteArray::Reserve(int32_t newCapacity) {
        return m_pImpl->_Reserve(newCapacity);
    }

    int32_t ByteArray::Capacity() const {
        return m_pImpl->_Capacity();
    }

    bool ByteArray::Empty() const {
        return 0 == m_pImpl->_Size();
    }

    SharedPtr<ByteArray> ByteArray::Slice(int32_t start, int32_t end) {
        if (end < 0) {
            end = m_pImpl->arrSize;
        }
        SharedPtr<ByteArray> arr(new ByteArray());
        if (0 <= start && start <= end && end <= m_pImpl->arrSize) {
            arr->m_pImpl->rawMem = m_pImpl->rawMem;
            arr->m_pImpl->arrStartPos = m_pImpl->arrStartPos + start;
            arr->m_pImpl->arrSize = end - start;
            arr->m_pImpl->pArray = arr->m_pImpl->rawMem->pBuf + arr->m_pImpl->arrStartPos;
        }
        return arr;
    }

    const SharedPtr<ByteArray> ByteArray::Slice(int32_t start, int32_t end) const {
        return const_cast<ByteArray *>(this)->Slice(start, end);
    }

    int32_t ByteArray::Copy(const uint8_t *pSrc, int32_t copyLen) {
        return m_pImpl->_Copy(pSrc, copyLen, 0, copyLen);
    }

    int32_t ByteArray::Copy(const ByteArray &src) {
        if (this == &src) {
            return m_pImpl->_Size();
        }
        return m_pImpl->_Copy(src.m_pImpl->rawMem->pBuf, src.m_pImpl->rawMem->bufSize,
                              src.m_pImpl->arrStartPos, src.m_pImpl->arrSize);
    }

    int32_t ByteArray::Assign(const uint8_t *pSrc, int32_t assignLen) {
        return m_pImpl->_Assign(pSrc, assignLen, 0, assignLen);
    }

    int32_t ByteArray::Assign(const ByteArray &src) {
        if (this == &src) {
            return m_pImpl->_Size();
        }
        return m_pImpl->_Assign(src.m_pImpl->rawMem->pBuf, src.m_pImpl->rawMem->bufSize,
                                src.m_pImpl->arrStartPos, src.m_pImpl->arrSize);
    }

    int32_t ByteArray::Share(uint8_t *pData, int32_t dataSize) {
        return m_pImpl->_Share(pData, dataSize, 0, dataSize);
    }

    int32_t ByteArray::Share(const ByteArray &src) {
        if (this != &src) {
            m_pImpl->rawMem = src.m_pImpl->rawMem;
            m_pImpl->arrStartPos = src.m_pImpl->arrStartPos;
            m_pImpl->arrSize = src.m_pImpl->arrSize;
            m_pImpl->pArray = m_pImpl->rawMem->pBuf + m_pImpl->arrStartPos;
            m_pImpl->end.m_pImpl->posInArr = m_pImpl->arrSize;
        }
        return m_pImpl->_Size();
    }

    int32_t ByteArray::Takeover(uint8_t *pSrc, int32_t srcLen) {
        return m_pImpl->_Takeover(pSrc, srcLen);
    }

    int32_t ByteArray::Takeover(ByteArray &other) {
        if (this != &other) {
            Share(other);
            other.m_pImpl->_Reset();
        }
        return m_pImpl->_Size();
    }

    int32_t ByteArray::Move(int32_t dst, int32_t src, int32_t dataSize) {
        return m_pImpl->_Move(dst, src, dataSize);
    }

    void ByteArray::Swap(ByteArray &other) {
        if (this != &other) {
            std::swap(m_pImpl, other.m_pImpl);
        }
    }

    void ByteArray::Fill(uint8_t val) {
        return m_pImpl->_Fill(val);
    }

    void ByteArray::Clear() {
        m_pImpl->arrSize = 0;
    }

    void ByteArray::Reset() {
        m_pImpl->_Reset();
    }

}
