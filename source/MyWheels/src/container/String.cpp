#include "InternalCommon.h"
#include "inc/String.h"
#include "inc/ByteArray.h"
#include "StringImplement.h"

#include <stdarg.h> // for va_start

namespace mwl {

#ifdef __MWL_WIN__
#define _SET_CNT_PTR \
    do { \
        pContent = m_pImpl ? m_pImpl->_pContent : nullptr; \
    } while(0)
#else
#define _SET_CNT_PTR
#endif

    // bufSize isn't include the ending '\0'
    String::Implement* _Allocate(int32_t bufSize, const char *pContent, void *freeThis = nullptr) {
        if (bufSize <= 0) {
            return nullptr;
        }
        String::Implement *pImpl = reinterpret_cast<String::Implement*>(calloc(String::Implement::_BASE_SIZE + bufSize + 1, 1));
        pImpl->_capacity = bufSize;
        pImpl->_len = 0;
        pImpl->_pContent = reinterpret_cast<char*>(pImpl) + String::Implement::_BASE_SIZE;
        if (pContent && *pContent) {
            int32_t len = strlen(pContent);
            if (len > pImpl->_capacity) {
                len = pImpl->_capacity;
            }
            pImpl->_Copy(pContent, len);
        }
        free(freeThis);
        return pImpl;
    }

    String::String() : m_pImpl(nullptr) {}

    String::String(const char *pStr, int32_t strLen) : m_pImpl(nullptr) {
        int32_t bufSize = 0;
        if (pStr && *pStr && strLen != 0) {
            bufSize = strlen(pStr);
            if (strLen > 0 && strLen < bufSize) {
                bufSize = strLen;
            }
        } else {
            pStr = nullptr;
        }
        m_pImpl = _Allocate(bufSize, pStr);
        _SET_CNT_PTR;
    }

    String::String(char c) : m_pImpl(nullptr) {
        if (c) {
            m_pImpl = _Allocate(1, &c);
        }
        _SET_CNT_PTR;
    }

    String::String(const String &src) : m_pImpl(nullptr) {
        if (!src.Empty()) {
            m_pImpl = _Allocate(src.Len(), src.C_Str());
        }
        _SET_CNT_PTR;
    }

    String::~String() {
        free(m_pImpl);
    }

    int32_t String::Len() const {
        return m_pImpl ? m_pImpl->_len : 0;
    }

    bool String::Empty() const {
        return Len() == 0;
    }

    const String& String::Null() {
        static const String nullStr("");
        return nullStr;
    }

    const char* String::C_Str() const {
        return Empty() ? "" : m_pImpl->_pContent;
    }

    String& String::operator=(const String &rhs) {
        if (this != &rhs) {
            if (!m_pImpl || m_pImpl->_len < rhs.Len()) {
                m_pImpl = _Allocate(rhs.Len(), rhs.C_Str(), m_pImpl);
            } else {
                m_pImpl->_Copy(rhs.C_Str(), rhs.Len());
            }
            _SET_CNT_PTR;
        }
        return *this;
    }

    String & String::operator+=(const String &rhs) {
        if (Empty() && !rhs.Empty()) {
            return operator=(rhs);
        } else if (!rhs.Empty()) {
            int32_t resultLen = m_pImpl->_len + rhs.m_pImpl->_len;
            bool selfDup = this == &rhs;
            if (m_pImpl->_capacity < resultLen) {
                m_pImpl = _Allocate(resultLen, m_pImpl->_pContent, m_pImpl);
                _SET_CNT_PTR;
            }
            if (selfDup) {
                m_pImpl->_Cat(m_pImpl->_pContent, m_pImpl->_len);
            } else {
                m_pImpl->_Cat(rhs.m_pImpl->_pContent, rhs.m_pImpl->_len);
            }
        }
        return *this;
    }

    void String::Clear() {
        if (m_pImpl) {
            m_pImpl->_pContent[0] = 0;
            m_pImpl->_len = 0;
        }
    }

    int32_t String::Compare(const String &other, CaseSensitivity caseSensitive) const {
        if (this == &other || (Empty() && other.Empty())) {
            return 0;
        } else if (Empty()) {
            return -1;
        } else if (other.Empty()) {
            return 1;
        } else {
            if (CASE_SENSITIVE == caseSensitive) {
                return strncmp(m_pImpl->_pContent, other.m_pImpl->_pContent, std::max(m_pImpl->_len, other.m_pImpl->_len));
            } else {
                return strncasecmp(m_pImpl->_pContent, other.m_pImpl->_pContent, std::max(m_pImpl->_len, other.m_pImpl->_len));
            }
        }
    }

    String& String::Cat(const String &other) {
        return operator+=(other);
    }

    String& String::Copy(const String &other) {
        return operator=(other);
    }

    const String& String::Format(const char *fmt, ...) {
        if (!fmt || !*fmt) {
            Clear();
        } else {
            int32_t res = 0;
            ByteArray resBuf(strlen(fmt) + 64);
            va_list va;
            do {
                va_start(va, fmt);
                res = vsnprintf(reinterpret_cast<char *>(resBuf.Data()), resBuf.Size() - 1, fmt, va);
                va_end(va);
                // if buffer is too small, vsnprintf will return -1 ( on Windows ) or actual lenth needed( on Linux )
                if (res < 0 || res >= resBuf.Size()) {
                    resBuf.Resize(resBuf.Size() + 64);
                    res = -1;
                }
            } while (res < 0);
            if (m_pImpl->_capacity < res) {
                m_pImpl = _Allocate(res, reinterpret_cast<char*>(resBuf.Data()), m_pImpl);
                _SET_CNT_PTR;
            } else {
                m_pImpl->_Copy(resBuf.Data(), res);
            }
        }
        return *this;
    }

    int32_t String::Extract(const char *fmt, ...) const {
        if (!fmt || !*fmt || Empty()) {
            return 0;
        } else {
            int32_t res = 0;
            va_list va;
            va_start(va, fmt);
            res = vsscanf(m_pImpl->_pContent, fmt, va);
            va_end(va);
            if (res == EOF) {
                res = -errno;
                MWL_WARN_ERRNO("vscanf failed", -res);
            }
            return res;
        }
    }

    const String String::SubString(int32_t start, int32_t subLen) const {
        if (Empty()) {
            return String();
        }

        int32_t len = m_pImpl->_len - start;
        if (len <= 0) {
            return String();
        }
        if (len > subLen) {
            len = subLen;
        }
        return String(m_pImpl->_pContent + start, subLen);
    }

    bool String::Contains(const String &subStr) const {
        return Find(subStr) >= 0;
    }

    int32_t String::Find(const String &subStr, int32_t start, int32_t end) const {
        if (end < 0 || end > Len()) {
            end = Len();
        }
        int32_t searchLen = end - start;
        if (subStr.Empty()) {
            return 0;
        } else if (subStr.m_pImpl->_len > searchLen) {
            return -1;
        }

        char tmp = m_pImpl->_pContent[end];
        m_pImpl->_pContent[end] = 0;
        char* p = strstr(m_pImpl->_pContent + start, subStr.m_pImpl->_pContent);
        m_pImpl->_pContent[end] = tmp;
        return p ? p - m_pImpl->_pContent : -1;
    }

    int32_t String::RFind(const String &subStr, int32_t start, int32_t end) const {
        if (end < 0 || end > Len()) {
            end = Len();
        }
        int32_t searchLen = end - start;
        if (subStr.Empty()) {
            return 0;
        } else if (subStr.m_pImpl->_len > searchLen) {
            return -1;
        }

        char tmp = m_pImpl->_pContent[end];
        m_pImpl->_pContent[end] = 0;
        char *pStr = m_pImpl->_pContent + start;
        char *pos = pStr + searchLen - subStr.m_pImpl->_len;
        char *p = nullptr;
        while (pos >= pStr && !p) {
            p = strstr(pos, subStr.m_pImpl->_pContent);
            pos -= 1;
        }
        m_pImpl->_pContent[end] = tmp;
        return p ? p - m_pImpl->_pContent : -1;
    }

    bool String::StartsWith(const String& prefix) const {
        if (prefix.Empty()) {
            return true;
        }
        if (Empty()) {
            return false;
        }
        if (prefix.m_pImpl->_len > m_pImpl->_len) {
            return false;
        }
        return 0 == strncmp(m_pImpl->_pContent, prefix.m_pImpl->_pContent, prefix.m_pImpl->_len);
    }

    bool String::EndsWith(const String& surfix) const {
        if (surfix.Empty()) {
            return true;
        }
        if (Empty()) {
            return false;
        }
        if (surfix.m_pImpl->_len > m_pImpl->_len) {
            return false;
        }
        return 0 == strncmp(m_pImpl->_pContent + m_pImpl->_len - surfix.m_pImpl->_len,
            surfix.m_pImpl->_pContent, surfix.m_pImpl->_len);
    }

    void String::Swap(String &other) {
        if (this != &other) {
            std::swap(m_pImpl, other.m_pImpl);
        }
    }

    static inline const String& _whitespaces() {
        static const char s_invisibleChars[] = {
            ' ', '\t', '\n', '\r', 0,
        };
        static const String whitespaces(s_invisibleChars);
        return whitespaces;
    }

    String& String::Strip(int32_t count) {
        return Strip(_whitespaces(), count);
    }

    String& String::Strip(const String &chars, int32_t count) {
        LStrip(chars, count);
        RStrip(chars, count);
        return*this;
    }

    String String::Stripped(int32_t count) const {
        return Stripped(_whitespaces(), count);
    }

    String String::Stripped(const String &chars, int32_t count) const {
        return LStripped(chars, count).RStrip(chars, count);
    }

    String& String::LStrip(int32_t count) {
        return LStrip(_whitespaces(), count);
    }

    String& String::LStrip(const String &chars, int32_t count) {
        if (count < 0) {
            count = Len();
        }
        if (!Empty() && !chars.Empty() && count > 0) {
            int32_t i = 0;
            int32_t cnt = 0;
            while (chars.Contains(m_pImpl->_pContent[i]) && i < m_pImpl->_len && cnt < count) {
                ++cnt;
                ++i;
            }
            m_pImpl->_len -= cnt;
            memmove(m_pImpl->_pContent, m_pImpl->_pContent + cnt, m_pImpl->_len);
            m_pImpl->_pContent[m_pImpl->_len] = 0;
        }
        return*this;
    }

    String String::LStripped(int32_t count) const {
        return LStripped(_whitespaces(), count);
    }

    String String::LStripped(const String &chars, int32_t count) const {
        return String(*this).LStrip(chars, count);
    }

    String& String::RStrip(int32_t count) {
        return RStrip(_whitespaces(), count);
    }

    String& String::RStrip(const String &chars, int32_t count) {
        if (count < 0) {
            count = Len();
        }
        if (!Empty() && !chars.Empty() && count > 0) {
            int32_t i = m_pImpl->_len - 1;
            int32_t cnt = 0;
            while (chars.Contains(m_pImpl->_pContent[i]) && i >= 0 && cnt < count) {
                ++cnt;
                --i;
            }
            m_pImpl->_len -= cnt;
            m_pImpl->_pContent[m_pImpl->_len] = 0;
        }
        return*this;
    }

    String String::RStripped(int32_t count) const {
        return RStripped(_whitespaces(), count);
    }

    String String::RStripped(const String &chars, int32_t count) const {
        return String(*this).RStrip(chars, count);
    }

    String& String::Replace(const String &origin, const String &replacement, int32_t count) {
        if (0 == count) {
            return *this;
        }
        if (origin != replacement) {
            // some special cases
            if (origin == *this) { // whole replacement
                return operator=(replacement);
            }
            if (origin.Empty()) { // add prefix and surfix
                if (count != 1) {
                    return operator=(replacement + *this + replacement);
                } else {
                    return operator=(replacement + *this);
                }
            }

            char *p = strstr(m_pImpl->_pContent, origin.m_pImpl->_pContent);
            if (p) { // 'origin' found
                if (count < 0) {
                    count = m_pImpl->_len;
                }
                int32_t origLen = origin.m_pImpl->_len;
                int32_t replLen = replacement.Len();
                int32_t replCnt = 0;
                // normal cases
                if (origLen >= replLen) {
                    int32_t gap = origLen - replLen;
                    char *pEnd = m_pImpl->_pContent + m_pImpl->_len;
                    while (p) {
                        if (replLen > 0) { // 'replacement' may be NullStr(), can't access its m_pImpl
                            memcpy(p, replacement.C_Str(), replLen);
                        }
                        if (gap > 0) {
                            memmove(p + replLen, p + origLen, pEnd - p - origLen);
                            m_pImpl->_len -= gap;
                            pEnd -= gap;
                            *pEnd = 0;
                        }
                        ++replCnt;
                        if (replCnt >= count) {
                            break;
                        }
                        p = strstr(p + replLen, origin.m_pImpl->_pContent);
                    }
                } else {
                    char *pStr = m_pImpl->_pContent;
                    String replaced(pStr, p - pStr);
                    bool done = false;
                    while (!done) {
                        replaced += replacement;
                        ++replCnt;
                        if (replCnt >= count) {
                            break;
                        }
                        pStr = p + origLen;
                        p = strstr(pStr, origin.m_pImpl->_pContent);
                        if (!p) {
                            p = m_pImpl->_pContent + m_pImpl->_len;
                            done = true;
                        }
                        replaced.m_pImpl = _Allocate(replaced.m_pImpl->_len + p - pStr,
                            replaced.m_pImpl->_pContent, replaced.m_pImpl);
                        replaced.m_pImpl->_Cat(pStr, p - pStr);
                    }
                    Swap(replaced);
                    _SET_CNT_PTR;
                }
            }
        }
        return*this;
    }

    Array<String> String::Split(int32_t count) const {
        Array<String> splitted;
        if (count < 0) {
            count = Len();
        }
        if (!Empty() && count > 0) {
            const char *pStr = m_pImpl->_pContent;
            int32_t splitCnt = 0;
            for (int32_t i = 0; i < m_pImpl->_len && splitCnt < count; ++i) {
                if (_whitespaces().Contains(m_pImpl->_pContent[i])) {
                    splitted.Append(String(pStr, m_pImpl->_pContent + i - pStr));
                    pStr = m_pImpl->_pContent + i + 1;
                    ++splitCnt;
                }
            }
            splitted.Append(pStr);
        }
        return splitted;
    }

    Array<String> String::Split(const String &seperator, int32_t count) const {
        Array<String> splitted;
        if (count < 0) {
            count = Len();
        }
        if (!Empty() && !seperator.Empty() && count > 0) {
            int32_t splitCnt = 0;
            char *pStr = m_pImpl->_pContent;
            char *p = strstr(m_pImpl->_pContent, seperator.m_pImpl->_pContent);
            while (p && splitCnt < count) {
                splitted.Append(String(pStr, p - pStr));
                ++splitCnt;
                pStr = p + seperator.m_pImpl->_len;
                p = strstr(pStr, seperator.m_pImpl->_pContent);
            }
            splitted.Append(pStr);
        }
        return splitted;
    }

    Array<String> String::RSplit(int32_t count) const {
        if (count < 0) {
            return Split(count);
        }
        
        Array<String> splitted;
        if (count < 0) {
            count = Len();
        }
        if (!Empty() && count > 0) {
            char *pStr = m_pImpl->_pContent + m_pImpl->_len - 1;
            char *p = m_pImpl->_pContent + m_pImpl->_len;
            int32_t splitCnt = 0;
            while (pStr >= m_pImpl->_pContent && splitCnt < count) {
                if (_whitespaces().Contains(*pStr)) {
                    splitted.Append(String(pStr + 1, p - pStr - 1));
                    ++splitCnt;
                    p = pStr;
                }
                --pStr;
            }

            splitted.Append(String(m_pImpl->_pContent, p - m_pImpl->_pContent));
            int32_t cnt = splitted.Size();
            for (int32_t i = 0; i < cnt / 2; ++i) {
                splitted[i].Swap(splitted[cnt - i - 1]);
            }
        }
        return splitted;
    }

    Array<String> String::RSplit(const String &seperator, int32_t count) const {
        if (count < 0) {
            return Split(seperator, count);
        }

        Array<String> splitted;
        if (!Empty() && !seperator.Empty() && count > 0) {
            if (m_pImpl->_len < seperator.m_pImpl->_len) {
                splitted.Append(*this);
            } else {
                int32_t splitCnt = 0;
                char *pEnd = m_pImpl->_pContent + m_pImpl->_len;
                char *pStr = m_pImpl->_pContent + m_pImpl->_len - seperator.m_pImpl->_len;
                while (pStr >= m_pImpl->_pContent && splitCnt < count) {
                    while (*pStr != seperator.m_pImpl->_pContent[0] &&
                        strncmp(pStr, seperator.m_pImpl->_pContent, seperator.m_pImpl->_len) != 0 &&
                        pStr >= m_pImpl->_pContent) {
                        --pStr;
                    }
                    splitted.Append(String(pStr + seperator.m_pImpl->_len, pEnd - pStr - seperator.m_pImpl->_len));
                    pEnd = pStr;
                    ++splitCnt;
                    --pStr;
                }
                splitted.Append(String(m_pImpl->_pContent, pEnd - m_pImpl->_pContent));
                int32_t cnt = splitted.Size();
                for (int32_t i = 0; i < cnt / 2; ++i) {
                    splitted[i].Swap(splitted[cnt - i - 1]);
                }
            }
        }
        return splitted;
    }

    String String::Join(const Array<String> &strings) const {
        String joined;
        if (!strings.Empty()) {
            joined = strings[0];
            for (int32_t i = 1; i < strings.Size(); ++i) {
                joined += *this + strings[i];
            }
        }
        return joined;
    }

    Array<String> String::Partition(const String &seperator) const {
        Array<String> splitted = Split(seperator, 1);
        if (splitted.Size() == 1) {
            splitted.Increase(2, Null());
        } else {
            splitted.Insert(1, seperator);
        }
        return splitted;
    }

}

mwl::String operator+(const mwl::String &lhs, const mwl::String &rhs) {
    return mwl::String(lhs).Cat(rhs);
}

bool operator==(const mwl::String &lhs, const mwl::String &rhs) {
    return 0 == lhs.Compare(rhs);
}

bool operator!=(const mwl::String &lhs, const mwl::String &rhs) {
    return !(lhs == rhs);
}

bool operator<(const mwl::String &lhs, const mwl::String &rhs) {
    return lhs.Compare(rhs) < 0;
}

bool operator<=(const mwl::String &lhs, const mwl::String &rhs) {
    return lhs.Compare(rhs) <= 0;
}

bool operator>(const mwl::String &lhs, const mwl::String &rhs) {
    return lhs.Compare(rhs) > 0;
}

bool operator>=(const mwl::String &lhs, const mwl::String &rhs) {
    return lhs.Compare(rhs) >= 0;
}
