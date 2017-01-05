#include "inc/ByteArray.h"


namespace mwl {

    ByteArray::ByteArray(int64_t initCapacity) {

    }

    ByteArray::ByteArray(const ByteArray &rhs) {

    }

    ByteArray::~ByteArray() {

    }

    ByteArray& ByteArray::operator=(const ByteArray & rhs) {
        return *this;
    }

    uint8_t ByteArray::operator[](int64_t idx) const {
        return 0;
    }

    uint8_t& ByteArray::operator[](int64_t idx) {
        static uint8_t dummy;
        return dummy; 
    }

    uint8_t* ByteArray::Begin() {
        return NULL;
    }

    const uint8_t* ByteArray::Begin() const {
        return NULL;
    }

    uint8_t* ByteArray::End() {
        return NULL;
    }

    const uint8_t* ByteArray::End() const {
        return NULL;
    }

    uint8_t* ByteArray::RawData(int64_t startIdx) {
        return NULL;
    }

    const uint8_t* ByteArray::RawData(int64_t startIdx) const {
        return NULL;
    }

    int64_t ByteArray::Reserve(int64_t newCapacity) {
        return 0;
    }

    int64_t ByteArray::Capacity() const {
        return 0;
    }

    int64_t ByteArray::Increase(int32_t deltaSize) {
        return 0;
    }

    int64_t ByteArray::Decrease(int32_t deltaSize) {
        return 0;
    }

    int64_t ByteArray::Resize(int64_t newSize) {
        return 0;
    }

    int64_t ByteArray::Size() const {
        return 0;
    }

    int32_t ByteArray::ShareConst(const uint8_t *pBuf, int64_t bufSize) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::ShareConst(const ByteArray &arr) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::ShareMutable(uint8_t *pBuf, int64_t bufSize) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::ShareMutable(ByteArray &arr) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::Copy(const uint8_t *pBuf, int64_t bufSize) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::Copy(const ByteArray &arr) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::Takeover(uint8_t *pBuf, int64_t bufSize) {
        return ERR_INVAL_OP;
    }

    int32_t ByteArray::Takeover(ByteArray &arr) {
        return ERR_INVAL_OP;
    }

    void ByteArray::Swap(ByteArray &arr) {

    }

    void ByteArray::Clear() {

    }

}
