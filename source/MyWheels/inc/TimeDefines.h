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
    };

    void MWL_API TimeSleep(double sleepTime, TimeUnit unit = MILLISEC);

}

#endif // __MWL_TIME_DEFINES_H__
