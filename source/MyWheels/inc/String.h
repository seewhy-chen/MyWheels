#ifndef __MWL_STRING_H__
#define __MWL_STRING_H__

#include "inc/BasicDefines.h"
#include "inc/SmartPointer.h"
#include "inc/Array.h"

namespace mwl {

    class MWL_API String {
    public:
        enum CaseSensitivity {
            CASE_SENSITIVE = 0L,
            CASE_INSENSITIVE = 1L,
        };

        static const String& Null();
        String();
        String(const char *pStr, int32_t strLen = -1);
        String(char c);
        String(const String &src);
        ~String();

        String& operator=(const String &rhs);
        String & operator+=(const String &rhs);

        const char *C_Str() const;
        int32_t Len() const;
        bool Empty() const;

        int32_t Compare(const String &other, CaseSensitivity caseSensitive = CASE_SENSITIVE) const;
        String& Cat(const String &other);
        String& Copy(const String &other);
        void Clear();

        const String& Format(const char *fmt, ...); // sprintf
        int32_t Extract(const char *fmt, ...) const; // sscanf

        String& Strip(int32_t count = -1);
        String& Strip(const String &chars, int32_t count = -1);
        String Stripped(int32_t count = -1) const;
        String Stripped(const String &chars, int32_t count = -1) const;
        String& LStrip(int32_t count = -1);
        String& LStrip(const String &chars, int32_t count = -1);
        String LStripped(int32_t count = -1) const;
        String LStripped(const String &chars, int32_t count = -1) const;
        String& RStrip(int32_t count = -1);
        String& RStrip(const String &chars, int32_t count = -1);
        String RStripped(int32_t count = -1) const;
        String RStripped(const String &chars, int32_t count = -1) const;

        const String SubString(int32_t start, int32_t subLen = -1) const;
        bool Contains(const String &subStr) const;
        int32_t Find(const String &subStr, int32_t start = 0, int32_t end = -1) const;
        int32_t RFind(const String &subStr, int32_t start = 0, int32_t end = -1) const;
        String& Replace(const String &origin, const String &replacement, int32_t count = -1);
        String Replaced(const String &origin, const String &replacement, int32_t count = -1) const;

        bool StartsWith(const String& prefix) const;
        bool EndsWith(const String& surfix) const;

        void Swap(String &other);

        Array<String> Split(int32_t count = -1) const;
        Array<String> Split(const String &seperator, int32_t count = -1) const;
        Array<String> RSplit(int32_t count = -1) const;
        Array<String> RSplit(const String &seperator, int32_t count = -1) const;
        Array<String> Partition(const String &seperator) const; // str --> head, sep, tail
        String Join(const Array<String> &strings) const;

        struct Implement;
    private:
        String(int64_t);
        String(uint64_t);
        String(int32_t);
        String(uint32_t);
        Implement* m_pImpl;
#ifdef __MWL_WIN__
        const char *pContent;
#endif
    };

    MWL_API mwl::String operator+(const mwl::String &lhs, const mwl::String &rhs);
    MWL_API bool operator==(const mwl::String &lhs, const mwl::String &rhs);
    MWL_API bool operator!=(const mwl::String &lhs, const mwl::String &rhs);
    MWL_API bool operator<(const mwl::String &lhs, const mwl::String &rhs);
    MWL_API bool operator<=(const mwl::String &lhs, const mwl::String &rhs);
    MWL_API bool operator>(const mwl::String &lhs, const mwl::String &rhs);
    MWL_API bool operator>=(const mwl::String &lhs, const mwl::String &rhs);

}

namespace std {
    inline void swap(mwl::String &s1, mwl::String &s2) {
        s1.Swap(s2);
    }
}

#endif // __MWL_STRING_H__
