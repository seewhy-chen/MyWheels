#ifndef __MWL_INTERNALCOMMON_H__
#define __MWL_INTERNALCOMMON_H__

#include "inc/BasicDefines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utility> // for std::swap
#include <algorithm> // for std::min
#include <errno.h>

#ifdef __MWL_WIN__

    // enable memory leakage detector
    #include <vld.h>

    #ifdef min
        #undef min
    #endif

    #ifdef max
        #undef max
    #endif

     // disable macro definition of 'min'
    #define NOMINMAX
    #include <Windows.h>

    #define snprintf sprintf_s
    #define strncasecmp _strnicmp

#elif defined __MWL_LINUX__
    #include <sys/types.h>
    #include <unistd.h>
    #include <pthread.h>

#endif

#endif // __MWL_INTERNALCOMMON_H__
