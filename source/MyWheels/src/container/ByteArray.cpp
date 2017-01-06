#include "inc/ByteArray.h"
#include "ByteArrayImplement.h"

namespace mwl {

    ByteArray::ByteArray(int64_t initCapacity, uint8_t initVal)
    : m_pImpl(new Implement(initCapacity, initVal)) {}

    ByteArray::ByteArray(const ByteArray &rhs)
    : m_pImpl(new Implement(*rhs.m_pImpl)) {}

    ByteArray::~ByteArray() {
        delete m_pImpl;
    }

    ByteArray & ByteArray::operator=(const ByteArray &rhs) {
        if (this != &rhs) {
            m_pImpl->impl->_Assign(rhs.RawData(), rhs.Size());
        }
        return *this;
    }

    uint8_t ByteArray::operator[](int64_t idx) const {
        return m_pImpl->impl->_ElementAt(idx);
    }

    uint8_t &ByteArray::operator[](int64_t idx) {
        return m_pImpl->impl->_ElementAt(idx);
    }

    uint8_t *ByteArray::Begin() {
        return m_pImpl->impl->_Begin();
    }

    const uint8_t *ByteArray::Begin() const {
        return m_pImpl->impl->_Begin();
    }

    uint8_t *ByteArray::End() {
        return m_pImpl->impl->_End();
    }

    const uint8_t *ByteArray::End() const {
        return m_pImpl->impl->_End();
    }

    uint8_t *ByteArray::RawData(int64_t startIdx) {
        return &m_pImpl->impl->_ElementAt(startIdx);
    }

    const uint8_t *ByteArray::RawData(int64_t startIdx) const {
        return &m_pImpl->impl->_ElementAt(startIdx);
    }

    int64_t ByteArray::Reserve(int64_t newCapacity) {
        return m_pImpl->impl->_Reserve(newCapacity);
    }

    int64_t ByteArray::Capacity() const {
        return m_pImpl->impl->_Capacity();
    }

    int64_t ByteArray::Increase(int32_t deltaSize, uint8_t initVal) {
        return m_pImpl->impl->_Resize(m_pImpl->impl->_Size() + deltaSize, initVal);
    }

    int64_t ByteArray::Decrease(int32_t deltaSize, uint8_t initVal) {
        return m_pImpl->impl->_Resize(m_pImpl->impl->_Size() - deltaSize, initVal);
    }

    int64_t ByteArray::Resize(int64_t newSize, uint8_t initVal) {
        return m_pImpl->impl->_Resize(newSize, initVal);
    }

    int64_t ByteArray::Shrink() {
        return m_pImpl->impl->_Shrink();
    }

    int64_t ByteArray::Size() const {
        return m_pImpl->impl->_Size();
    }

    int32_t ByteArray::Share(uint8_t *pBuf, int64_t bufSize) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::Share(ByteArray &arr) {
        m_pImpl->impl = arr.m_pImpl->impl;
        return ERR_NONE;
    }

    int32_t ByteArray::Assign(const uint8_t *pData, int64_t dataSize) {
        return m_pImpl->impl->_Assign(pData, dataSize);
    }

    int32_t ByteArray::Assign(const ByteArray &arr) {
        return m_pImpl->impl->_Assign(arr.RawData(), arr.Size());
    }

    int32_t ByteArray::Takeover(uint8_t *pBuf, int64_t bufSize) {
        return m_pImpl->impl->_Takeover(pBuf, bufSize);
    }

    int32_t ByteArray::Takeover(ByteArray &arr) {
        m_pImpl->impl.reset(arr.m_pImpl->impl.get());
        arr.m_pImpl->impl.reset();
        return m_pImpl->impl->_Takeover(arr);
    }

    void ByteArray::Swap(ByteArray &other) {
        m_pImpl->impl.swap(other.m_pImpl->impl);
    }

    void ByteArray::Clear() {
        m_pImpl->impl->_Clear();
    }

}
