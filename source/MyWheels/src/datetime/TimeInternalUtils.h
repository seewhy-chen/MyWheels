#ifndef __TIME_INTERNAL_UTILS_H__
#define __TIME_INTERNAL_UTILS_H__

#include "inc/TimeDefines.h"
#include <time.h>

namespace mwl {
#ifdef __MWL_WIN__
#define strptime mwl_strptime
    char *mwl_strptime(const char *buf, const char *fmt, tm *tm);
#endif
}

#endif // __TIME_INTERNAL_UTILS_H__