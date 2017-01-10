#include "inc/ByteArray.h"
#include "ByteArrayImplement.h"

#include <utility> // for std::swap

namespace mwl {

    ByteArray::ByteArray(int64_t initSize, uint8_t initVal)
    : m_pImpl(new Implement(initSize, initVal)) {}

    ByteArray::ByteArray(uint8_t *pData, int64_t dataSize, OwnerShip ownership)
    : m_pImpl(new Implement(pData, dataSize, ownership)) {}

    ByteArray::ByteArray(const uint8_t *pData, int64_t dataSize) 
    : m_pImpl(new Implement(const_cast<uint8_t*>(pData), dataSize, OWN_COPY)) {}

    ByteArray::ByteArray(const ByteArray &rhs)
    : m_pImpl(new Implement(const_cast<uint8_t*>(rhs.Data()), rhs.Size(), OWN_COPY)) {}

    ByteArray::~ByteArray() {
        delete m_pImpl;
    }

    ByteArray & ByteArray::operator=(const ByteArray &rhs) {
        if (this != &rhs) {
            m_pImpl->_Assign(rhs.Data(), rhs.Size());
        }
        return *this;
    }

    uint8_t ByteArray::operator[](int64_t idx) const {
        return m_pImpl->_ElementAt(idx);
    }

    uint8_t &ByteArray::operator[](int64_t idx) {
        return m_pImpl->_ElementAt(idx);
    }

    uint8_t ByteArray::At(int64_t idx) const {
        return m_pImpl->_ElementAt(idx);
    }

    uint8_t& ByteArray::At(int64_t idx) {
        return m_pImpl->_ElementAt(idx);
    }

    uint8_t *ByteArray::Begin() {
        return m_pImpl->_Begin();
    }

    const uint8_t *ByteArray::Begin() const {
        return m_pImpl->_Begin();
    }

    uint8_t *ByteArray::End() {
        return m_pImpl->_End();
    }

    const uint8_t *ByteArray::End() const {
        return m_pImpl->_End();
    }

    uint8_t *ByteArray::Data(int64_t startIdx) {
        return &m_pImpl->_ElementAt(startIdx);
    }

    const uint8_t *ByteArray::Data(int64_t startIdx) const {
        return &m_pImpl->_ElementAt(startIdx);
    }

    bool ByteArray::Empty() const {
        return 0 == m_pImpl->_Size();
    }

    int64_t ByteArray::Size() const {
        return m_pImpl->_Size();
    }

    int64_t ByteArray::Copy(const uint8_t *pData, int64_t dataSize) {
        return m_pImpl->_Copy(pData, dataSize);
    }

    int64_t ByteArray::Assign(const uint8_t *pData, int64_t dataSize) {
        return m_pImpl->_Assign(pData, dataSize);
    }

    int64_t ByteArray::Share(uint8_t *pData, int64_t dataSize) {
        return m_pImpl->_Share(pData, dataSize);
    }

    int64_t ByteArray::Takeover(uint8_t *pData, int64_t dataSize) {
        return m_pImpl->_Takeover(pData, dataSize);
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
        return m_pImpl->_Reset();
    }

}
