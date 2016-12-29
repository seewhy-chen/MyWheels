#ifndef __MWL_BASIC_DEFINES_H__
#define __MWL_BASIC_DEFINES_H__

#include <stdio.h>
#include <errno.h>

#ifdef _MSC_VER

#define __MWL_WIN__
#include <winerror.h>

#ifndef ETIMEDOUT
#define ETIMEDOUT WSAETIMEDOUT
#endif

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

#define strerror_r(err, buf, buflen) strerror_s(buf, buflen, err)

#elif defined __GNUC__

#define __MWL_LINUX__

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

    enum ErrorCode {
        ERR_NONE = 0L,
        ERR_INVAL_PARAM = -EINVAL,
        ERR_AGAIN = -EAGAIN,
        ERR_TIMEOUT = -ETIMEDOUT,
    };

#define MWL_INFO(fmt, ...) \
    do { \
        fprintf(stdout, "[Info] " fmt"\n", ##__VA_ARGS__); \
    } while (0)


#define MWL_WARN(fmt, ...) \
    do { \
        fprintf(stdout, "[Warn] " fmt"\n", ##__VA_ARGS__); \
    } while (0)

#define MWL_WARN_ERRNO(fmt, err, ...) \
    do { \
        char errMsg[256] = {0}; \
        strerror_r(err, errMsg, sizeof(errMsg)); \
        fprintf(stdout, "[Warn] " fmt": %s(%d)\n", ##__VA_ARGS__, errMsg, err); \
    } while (0)

#define MWL_ERROR(fmt, ...) \
    do { \
        fprintf(stdout, "[Error] " fmt"\n", ##__VA_ARGS__); \
    } while (0)

#define MWL_ERROR_ERRNO(fmt, err, ...) \
    do { \
        char errMsg[256] = {0}; \
        strerror_r(err, errMsg, sizeof(errMsg)); \
        fprintf(stdout, "[Error] " fmt": %s(%d)\n", ##__VA_ARGS__, errMsg, err); \
    } while (0)

}

#endif // __MWL_BASIC_DEFINES_H__
