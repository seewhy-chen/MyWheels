#define MWL_LOG_TAG "String"

#include "StringImplement.h"
#include <algorithm>

namespace mwl {
    const int32_t String::Implement::_BASE_SIZE = sizeof(String::Implement);
    void String::Implement::_Copy(const void* pSrc, int32_t len) {
        strncpy(_pContent, reinterpret_cast<const char*>(pSrc), len);
        _pContent[len] = 0;
        _len = len;
    }

    void String::Implement::_Cat(const void* pSrc, int32_t len) {
        strncat(_pContent, reinterpret_cast<const char*>(pSrc), len);
        _pContent[_len + len] = 0;
        _len += len;
    }
}
