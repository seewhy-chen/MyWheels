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

        String(const char *pStr);
        String(char c);
        String(const String &src);
        ~String();

        String& operator=(const String &rhs);
        const String operator+(const String &rhs) const;
        const String & operator+=(const String &rhs);

        void Clear();
        int32_t Assign(const String &other);
        int32_t Cat(const String &other);
        int32_t Copy(const String &other);

        Array<String> Split(const String &seperator, int32_t count = -1);
        Array<String> RSplit(const String &seperator, int32_t count = -1);

        const String& Join(const Array<String> &strings);
        const String& Format(const String &fmt, ...); // sprintf
        int32_t Extract(const String &fmt, ...) const; // sscanf

        String& Strip(const String &chars = String::Null());
        String Stripped(const String &chars = String::Null()) const;
        String& LStrip(const String &chars = String::Null());
        String LStripped(const String &chars = String::Null()) const;
        String& RStrip(const String &chars = String::Null());
        String RStripped(const String &chars = String::Null()) const;
        Array<String> Partition(const String &seperator); // str --> head, sep, tail

        const String& SubString(int32_t start, int32_t end = -1) const;
        bool Contains(const String &subStr, CaseSensitivity caseSensitive = CASE_SENSITIVE) const;
        int32_t Find(const String &subStr, CaseSensitivity caseSensitive = CASE_SENSITIVE) const;
        int32_t RFind(const String &subStr, CaseSensitivity caseSensitive = CASE_SENSITIVE) const;
        String& Replace(const String &origin, const String &replacement, int32_t count = 1);

        const char *C_Str() const;
        int32_t Len() const;
        bool Empty() const;
        bool NotEmpty() const;
        bool StartsWith(const String& prefix) const;
        bool EndsWith(const String& surfix) const;

        void Swap(String &other);
        static const String& Null();

    private:
        struct Implement;
        SharedPtr<Implement> m_pImpl;
    };

}

#endif // __MWL_STRING_H__
