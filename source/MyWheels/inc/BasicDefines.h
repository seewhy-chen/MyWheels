#ifndef __MWL_BASIC_DEFINES_H__
#define __MWL_BASIC_DEFINES_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef _MSC_VER

#define __MWL_WIN__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
    #define nullptr 0
#else
    #include <stdint.h>
#endif

#ifndef ETIMEDOUT
    #define ETIMEDOUT WSAETIMEDOUT
#endif

#define ZD "lu"
#define JD "llu"

#include <ws2tcpip.h>
inline static char *strerror_r(int32_t err, char *buf, int32_t buflen) {
    strcpy_s(buf, buflen, gai_strerror(err));
    return buf;
}

#elif defined __GNUC__

#define __MWL_LINUX__

#if defined(MWL_MAKE_SHARED_LIB)
    #define MWL_API   __attribute__((visibility("default")))
#else
    #define MWL_API
#endif  // MWL_MAKE_SHARED_LIB

#include <stdint.h>

#define ZD "zd"
#define JD "jd"

#else

#error unknown compiler!

#endif  // _MSC_VER


#if __cplusplus >= 201103L
    #define __CXX_11__
#endif

#include "inc/StaticAssert.h"
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
        ERR_PERM = -EPERM,
        ERR_AGAIN = -EAGAIN,
        ERR_NOMEM = -ENOMEM,
        ERR_BUSY = -EBUSY,
        ERR_INVAL_PARAM = -EINVAL,
        ERR_TIMEOUT = -ETIMEDOUT,

        ERR_INVAL_OP = -0x10000L,
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
        char *msg = strerror_r(err, errMsg, sizeof(errMsg)); \
        fprintf(stdout, "[Warn] " fmt": %d -- %s\n", ##__VA_ARGS__, err, msg); \
    } while (0)

#define MWL_ERR(fmt, ...) \
    do { \
        fprintf(stdout, "[Error] " fmt"\n", ##__VA_ARGS__); \
    } while (0)

#define MWL_ERR_ERRNO(fmt, err, ...) \
    do { \
        char errMsg[256] = {0}; \
        char *msg = strerror_r(err, errMsg, sizeof(errMsg)); \
        fprintf(stdout, "[Error] " fmt": %d -- %s\n", ##__VA_ARGS__, err, msg); \
    } while (0)

#define MWL_ARR_SIZE(arr) (static_cast<int32_t>(sizeof((arr))/sizeof((arr)[0])))

    enum OwnerShip {
        OWN_COPY,
        OWN_SHARE,
        OWN_TAKEOVER,
    };

} // namespace mwl

#endif // __MWL_BASIC_DEFINES_H__
