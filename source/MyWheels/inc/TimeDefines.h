#ifndef __MWL_TIME_DEFINES_H__
#define __MWL_TIME_DEFINES_H__

#include "inc/BasicDefines.h"

namespace mwl {

    enum TimeUnit {
        HOUR,
        MINUTE,
        SECOND,
        MILLISEC,
        MICROSEC,
        NANOSEC,

        TimeUnitCount
    };
}

#endif // __MWL_TIME_DEFINES_H__
