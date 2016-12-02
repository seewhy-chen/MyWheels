#ifndef __MWL_BASIC_DEFINES_H__
#define __MWL_BASIC_DEFINES_H__

#include <stdio.h>

#ifdef _MSC_VER

#define __WINDOWS__

#if defined(MWL_MAKE_SHARED_LIB)
    #define MWL_API  __declspec(dllexport)
#else
    #define MWL_API  __declspec(dllimport)
#endif  // MWL_MAKE_SHARED_LIB

#if _MSC_VER < 1900
    typedef signed char int8_t;
    typedef unsigned char uint8_t;
    typedef signed short int16_t;
    typedef unsigned short uint16_t;
    typedef signed int int32_t;
    typedef unsigned int uint32_t;
    typedef signed long long int64_t;
    typedef unsigned long long uint64_t;
#else
    #include <stdint.h>
#endif

#elif defined __GNUC__

#define __LINUX__

#if defined(MWL_MAKE_SHARED_LIB)
    #define MWL_API   __attribute__((visibility("default")))
#else
    #define MWL_API
#endif  // MWL_MAKE_SHARED_LIB

#include <stdint.h>

#else

#error unknown compiler!

#endif  // _MSC_VER

#if __cplusplus >= 201103L
#define __CXX_11__
#endif

namespace mwl {
    class MWL_API NonCopyable {
#ifdef __CXX_11__
    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
    private:
        NonCopyable(const NonCopyable &) = delete;
        NonCopyable &operator=(const NonCopyable &) = delete;
#else
    protected:
        NonCopyable() {}
        ~NonCopyable() {}
    private:
        NonCopyable(const NonCopyable &);
        NonCopyable &operator=(const NonCopyable &);
#endif // __CXX_11__
    };


#define MWL_LOG(fmt, ...) \
    do { \
        fprintf(stdout, fmt"\n", ##__VA_ARGS__); \
    } while (0)

#define MWL_ERR(fmt, ...) \
    do { \
        fprintf(stdout, "[Error] " fmt"\n", ##__VA_ARGS__); \
    } while (0)

}

#endif // __MWL_BASIC_DEFINES_H__
