#ifndef __MWL_INTERNALCOMMON_H__
#define __MWL_INTERNALCOMMON_H__

#include "inc/BasicDefines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utility> // for std::swap
#include <algorithm> // for std::min

#ifdef __MWL_WIN__

    #ifdef min
        #undef min
    #endif

    #ifdef max
        #undef max
    #endif

    #define NOMINMAX // disable macro definition of 'min'
    #include <Windows.h>

#elif defined __MWL_LINUX__

    #include <sys/types.h>
    #include <unistd.h>
    #include <pthread.h>

#endif

#endif // __MWL_INTERNALCOMMON_H__
