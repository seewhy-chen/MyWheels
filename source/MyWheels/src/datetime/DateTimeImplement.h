#ifndef __MWL_DATETIME_IMPLEMENT_H__
#define __MWL_DATETIME_IMPLEMENT_H__

#include "inc/DateTime.h"
#include <time.h>

#ifndef __MWL_WIN__
#include <sys/time.h>
#else
#include <sys/types.h>
#include <sys/timeb.h>
#endif

namespace mwl {

    static const char *s_defaultFmt = "%x %X";
    struct DateTime::Implement {
        Implement(bool isUTC);
        int32_t _FromStr(const String &timeString, const String &fmt);
        bool _utc;
        timeval _tv;
        tm _tm;
        String _defaultFmt;
    };

    static inline void _GetTimeOfDay(timeval *pTv) {
#if defined(__MWL_WIN__)
        __timeb64 tb;
        _ftime64_s(&tb);
        pTv->tv_sec = static_cast<long>(tb.time);
        pTv->tv_usec = tb.millitm * 1000;
#else
        gettimeofday(pTv, NULL);
#endif
    }

    static inline void _Tv2Tm(const timeval *pTv, tm *pTm, bool isUTC) {
#if defined(__MWL_WIN__)
        __time64_t t = pTv->tv_sec + (pTv->tv_usec / 1000000);
        if (isUTC) {
            _gmtime64_s(pTm, &t);
        } else {
            _localtime64_s(pTm, &t);
        }
#else
        if (isUTC) {
            gmtime_r(&pTv->tv_sec, pTm);
        } else {
            localtime_r(&pTv->tv_sec, pTm);
        }
#endif
    }

    static inline void _Tm2Tv(tm *pTm, timeval *pTv, bool isUTC) {
#if defined(__MWL_WIN__)
        __time64_t t = isUTC ? _mkgmtime64(pTm) : _mktime64(pTm);
        pTv->tv_sec = static_cast<long>(t);
#else
        time_t t = isUTC ? timegm(pTm) : mktime(pTm);
        pTv->tv_sec = t;
#endif
        pTv->tv_usec = 0;
    }

}

#endif // __MWL_DATETIME_IMPLEMENT_H__
