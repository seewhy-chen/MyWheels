#include "InternalCommon.h"

#include "inc/ByteArray.h"
#include "ByteArrayImplement.h"

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

    int64_t ByteArray::Copy(const ByteArray &data, int64_t dataSize, int64_t startPos) {
        if (startPos < 0 || startPos >= data.Size()) {
            return ERR_INVAL_PARAM;
        }

        int64_t dataToOp = std::min(data.Size() - startPos, dataSize);
        if (dataToOp <= 0 ) {
            return 0;
        }
        if (this != &data) {
            return m_pImpl->_Copy(data.Data(startPos), dataToOp);
        } else {
            return m_pImpl->_Move(0, startPos, dataToOp);
        }
    }

    int64_t ByteArray::Assign(const uint8_t *pData, int64_t dataSize) {
        return m_pImpl->_Assign(pData, dataSize);
    }

    int64_t ByteArray::Assign(const ByteArray &data, int64_t dataSize, int64_t startPos) {
        if (startPos < 0 || startPos >= data.Size()) {
            return ERR_INVAL_PARAM;
        }

        int64_t dataToOp = std::min(data.Size() - startPos, dataSize);
        if (dataToOp <= 0 ) {
            m_pImpl->_Reset();
            return 0;
        } 
        if (this != &data) {
            return m_pImpl->_Assign(data.Data(startPos), dataToOp);
        } else {
            m_pImpl->_Move(0, startPos, dataToOp);
            m_pImpl->bufSize = dataToOp;
            return dataToOp;
        }
    }

    int64_t ByteArray::Share(uint8_t *pData, int64_t dataSize) {
        return m_pImpl->_Share(pData, dataSize);
    }

    int64_t ByteArray::Share(ByteArray &data, int64_t dataSize, int64_t startPos) {
        if (startPos < 0 || startPos >= data.Size()) {
            return ERR_INVAL_PARAM;
        }
        if (&data == this) {
            return ERR_INVAL_OP;
        }
        int64_t dataToOp = std::min(data.Size() - startPos, dataSize);
        return m_pImpl->_Share(data.Data(startPos), dataToOp);
    }

    int64_t ByteArray::Takeover(uint8_t *pData, int64_t dataSize) {
        return m_pImpl->_Takeover(pData, dataSize);
    }

    int64_t ByteArray::Takeover(ByteArray &data) {
        m_pImpl->_Reset();
        Swap(data);
        return m_pImpl->_Size();
    }


    int64_t ByteArray::Move(int64_t dst, int64_t src, int64_t dataSize) {
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
        return m_pImpl->_Reset();
    }

}
