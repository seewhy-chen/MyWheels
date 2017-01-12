#include "inc/ByteArray.h"
#include "ByteArrayImplement.h"

namespace mwl {

#define MWL_IMPL_IT_COMMON_METHODS(ItClass) \
    ByteArray::ItClass::ItClass() \
    : m_pImpl(new ByteArray::IteratorImplement()) { \
    } \
    ByteArray::ItClass::ItClass(const ByteArray::ItClass &src) \
    : m_pImpl(new ByteArray::IteratorImplement()) { \
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl; \
        m_pImpl->posInArr = src.m_pImpl->posInArr; \
    } \
    ByteArray::ItClass::~ItClass() { \
        delete m_pImpl; \
    } \
    ByteArray::ItClass &ByteArray::ItClass::operator=(const ByteArray::ItClass &rhs) { \
        if (this != &rhs) { \
            m_pImpl->pArrImpl = rhs.m_pImpl->pArrImpl; \
            m_pImpl->posInArr = rhs.m_pImpl->posInArr; \
        } \
        return *this; \
    } \
    ByteArray::ItClass ByteArray::ItClass::operator++(int32_t) { \
        ByteArray::ItClass it(*this); \
        ++(*this); \
        return it; \
    } \
    ByteArray::ItClass ByteArray::ItClass::operator--(int32_t) { \
        ByteArray::ItClass it(*this); \
        --(*this); \
        return it; \
    } \
    ByteArray::ItClass ByteArray::ItClass::operator+(int32_t step) const { \
        ByteArray::ItClass it(*this); \
        it += step; \
        return it; \
    } \
    ByteArray::ItClass ByteArray::ItClass::operator-(int32_t step) const { \
        ByteArray::ItClass it(*this); \
        it -= step; \
        return it; \
    } \
    bool ByteArray::ItClass::operator==(const ByteArray::ItClass &rhs) const { \
        return this == &rhs || \
               (m_pImpl->pArrImpl->rawMem->pBuf == rhs.m_pImpl->pArrImpl->rawMem->pBuf && \
                m_pImpl->pArrImpl->arrStartPos + m_pImpl->posInArr == rhs.m_pImpl->pArrImpl->arrStartPos + rhs.m_pImpl->posInArr); \
    } \
    bool ByteArray::ItClass::operator!=(const ByteArray::ItClass &rhs) const { \
        return !(*this == rhs); \
    } \
    bool ByteArray::ItClass::operator<(const ByteArray::ItClass &rhs) const { \
        return m_pImpl->posInArr < rhs.m_pImpl->posInArr; \
    } \
    bool ByteArray::ItClass::operator>(const ByteArray::ItClass &rhs) const { \
        return m_pImpl->posInArr > rhs.m_pImpl->posInArr; \
    } \
    bool ByteArray::ItClass::operator<=(const ByteArray::ItClass &rhs) const { \
        return *this == rhs || *this < rhs; \
    } \
    bool ByteArray::ItClass::operator>=(const ByteArray::ItClass &rhs) const { \
        return *this == rhs || *this > rhs; \
    } 

    MWL_IMPL_IT_COMMON_METHODS(Iterator)

    ByteArray::Iterator::Iterator(const ByteArray::ReverseIterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr + 1;
    }

    uint8_t &ByteArray::Iterator::operator*() const {
        return m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    uint8_t *ByteArray::Iterator::operator->() const {
        return &m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    ByteArray::Iterator &ByteArray::Iterator::operator++() {
        ++m_pImpl->posInArr;
        return *this;
    }

    ByteArray::Iterator &ByteArray::Iterator::operator--() {
        --m_pImpl->posInArr;
        return *this;
    }

    ByteArray::Iterator &ByteArray::Iterator::operator+=(int32_t step) {
        m_pImpl->posInArr += step;
        return *this;
    }

    ByteArray::Iterator &ByteArray::Iterator::operator-=(int32_t step) {
        m_pImpl->posInArr -= step;
        return *this;
    }

    MWL_IMPL_IT_COMMON_METHODS(ConstIterator)
    ByteArray::ConstIterator::ConstIterator(const ByteArray::Iterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr;
    }
    ByteArray::ConstIterator::ConstIterator(const ByteArray::ConstReverseIterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr + 1;
    }

    ByteArray::ConstIterator::ConstIterator(const ByteArray::ReverseIterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr + 1;
    }

    const uint8_t &ByteArray::ConstIterator::operator*() const {
        return m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    const uint8_t *ByteArray::ConstIterator::operator->() const {
        return &m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    ByteArray::ConstIterator &ByteArray::ConstIterator::operator++() {
        ++m_pImpl->posInArr;
        return *this;
    }

    ByteArray::ConstIterator &ByteArray::ConstIterator::operator--() {
        --m_pImpl->posInArr;
        return *this;
    }

    ByteArray::ConstIterator &ByteArray::ConstIterator::operator+=(int32_t step) {
        m_pImpl->posInArr += step;
        return *this;
    }

    ByteArray::ConstIterator &ByteArray::ConstIterator::operator-=(int32_t step) {
        m_pImpl->posInArr -= step;
        return *this;
    }


    MWL_IMPL_IT_COMMON_METHODS(ReverseIterator)
    ByteArray::ReverseIterator::ReverseIterator(const ByteArray::Iterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr - 1;
    }

    uint8_t &ByteArray::ReverseIterator::operator*() const {
        return m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    uint8_t *ByteArray::ReverseIterator::operator->() const {
        return &m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    ByteArray::ReverseIterator &ByteArray::ReverseIterator::operator++() {
        --m_pImpl->posInArr;
        return *this;
    }

    ByteArray::ReverseIterator &ByteArray::ReverseIterator::operator--() {
        ++m_pImpl->posInArr;
        return *this;
    }

    ByteArray::ReverseIterator &ByteArray::ReverseIterator::operator+=(int32_t step) {
        m_pImpl->posInArr -= step;
        return *this;
    }

    ByteArray::ReverseIterator &ByteArray::ReverseIterator::operator-=(int32_t step) {
        m_pImpl->posInArr += step;
        return *this;
    }


    MWL_IMPL_IT_COMMON_METHODS(ConstReverseIterator)
    ByteArray::ConstReverseIterator::ConstReverseIterator(const ByteArray::ReverseIterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr;
    }

    ByteArray::ConstReverseIterator::ConstReverseIterator(const ByteArray::Iterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr - 1;
    }

    ByteArray::ConstReverseIterator::ConstReverseIterator(const ByteArray::ConstIterator &src)
    : m_pImpl(new ByteArray::IteratorImplement()) {
        m_pImpl->pArrImpl = src.m_pImpl->pArrImpl;
        m_pImpl->posInArr = src.m_pImpl->posInArr - 1;
    }

    const uint8_t &ByteArray::ConstReverseIterator::operator*() const {
        return m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    const uint8_t *ByteArray::ConstReverseIterator::operator->() const {
        return &m_pImpl->pArrImpl->_ElementAt(m_pImpl->posInArr);
    }

    ByteArray::ConstReverseIterator &ByteArray::ConstReverseIterator::operator++() {
        --m_pImpl->posInArr;
        return *this;
    }

    ByteArray::ConstReverseIterator &ByteArray::ConstReverseIterator::operator--() {
        ++m_pImpl->posInArr;
        return *this;
    }

    ByteArray::ConstReverseIterator &ByteArray::ConstReverseIterator::operator+=(int32_t step) {
        m_pImpl->posInArr -= step;
        return *this;
    }

    ByteArray::ConstReverseIterator &ByteArray::ConstReverseIterator::operator-=(int32_t step) {
        m_pImpl->posInArr += step;
        return *this;
    }

}
