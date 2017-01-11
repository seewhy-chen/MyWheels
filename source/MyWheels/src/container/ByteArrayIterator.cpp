#include "inc/ByteArray.h"
#include "ByteArrayImplement.h"

namespace mwl {

    ByteArray::Iterator::Iterator() {
    }

    ByteArray::Iterator::Iterator(const ByteArray::Iterator &src) {
    }

    ByteArray::Iterator::~Iterator() {
    }

    ByteArray::Iterator& ByteArray::Iterator::operator=(const ByteArray::Iterator &rhs) {
        return *this;
    }

    uint8_t s_dummy;
    uint8_t& ByteArray::Iterator::operator*() const {
        return s_dummy;
    }

    uint8_t* ByteArray::Iterator::operator->() const {
        return &s_dummy;
    }

    ByteArray::Iterator& ByteArray::Iterator::operator++() {
        return *this;
    }

    ByteArray::Iterator ByteArray::Iterator::operator++(int32_t) {
        return *this;
    }

    ByteArray::Iterator& ByteArray::Iterator::operator--() {
        return *this;
    }

    ByteArray::Iterator ByteArray::Iterator::operator--(int32_t) {
        return *this;
    }


    ByteArray::Iterator& ByteArray::Iterator::operator+=(int32_t step) {
        return *this;
    }

    ByteArray::Iterator& ByteArray::Iterator::operator-=(int32_t step) {
        return *this;
    }


    ByteArray::Iterator ByteArray::Iterator::operator+(int32_t step) const {
        return *this;
    }

    ByteArray::Iterator ByteArray::Iterator::operator-(int32_t step) const {
        return *this;
    }


    bool ByteArray::Iterator::operator<(const ByteArray::Iterator &rhs) const {
        return false;
    }

    bool ByteArray::Iterator::operator>(const ByteArray::Iterator &rhs) const {
        return false;
    }

    bool ByteArray::Iterator::operator<=(const ByteArray::Iterator &rhs) const {
        return false;
    }

    bool ByteArray::Iterator::operator>=(const ByteArray::Iterator &rhs) const {
        return false;
    }

    bool ByteArray::Iterator::operator==(const ByteArray::Iterator &rhs) const {
        return false;
    }

    bool ByteArray::Iterator::operator!=(const ByteArray::Iterator &rhs) const {
        return false;
    }


    ByteArray::ConstIterator::ConstIterator() {
    }

    ByteArray::ConstIterator::ConstIterator(const ByteArray::ConstIterator &src) {
    }

    ByteArray::ConstIterator::ConstIterator(const ByteArray::Iterator &src) {
    }

    ByteArray::ConstIterator::~ConstIterator() {
    }

    ByteArray::ConstIterator& ByteArray::ConstIterator::operator=(const ByteArray::ConstIterator &rhs) {
        return *this;
    }

    uint8_t& ByteArray::ConstIterator::operator*() const {
        return s_dummy;
    }

    uint8_t* ByteArray::ConstIterator::operator->() const {
        return &s_dummy;
    }

    ByteArray::ConstIterator& ByteArray::ConstIterator::operator++() {
        return *this;
    }

    ByteArray::ConstIterator ByteArray::ConstIterator::operator++(int32_t) {
        return *this;
    }

    ByteArray::ConstIterator& ByteArray::ConstIterator::operator--() {
        return *this;
    }

    ByteArray::ConstIterator ByteArray::ConstIterator::operator--(int32_t) {
        return *this;
    }

    ByteArray::ConstIterator& ByteArray::ConstIterator::operator+=(int32_t step) {
        return *this;
    }

    ByteArray::ConstIterator& ByteArray::ConstIterator::operator-=(int32_t step) {
        return *this;
    }

    ByteArray::ConstIterator ByteArray::ConstIterator::operator+(int32_t step) const {
        return *this;
    }

    ByteArray::ConstIterator ByteArray::ConstIterator::operator-(int32_t step) const {
        return *this;
    }

    bool ByteArray::ConstIterator::operator<(const ByteArray::ConstIterator &rhs) const {
        return false;
    }

    bool ByteArray::ConstIterator::operator>(const ByteArray::ConstIterator &rhs) const {
        return false;
    }

    bool ByteArray::ConstIterator::operator<=(const ByteArray::ConstIterator &rhs) const {
        return false;
    }

    bool ByteArray::ConstIterator::operator>=(const ByteArray::ConstIterator &rhs) const {
        return false;
    }

    bool ByteArray::ConstIterator::operator==(const ByteArray::ConstIterator &rhs) const {
        return false;
    }

    bool ByteArray::ConstIterator::operator!=(const ByteArray::ConstIterator &rhs) const {
        return false;
    }
    
}
