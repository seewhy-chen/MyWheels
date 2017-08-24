#include "inc/TimeUtils.h"
#include <time.h>

#ifndef __MWL_WIN__
#include <unistd.h> // for usleep
#endif

namespace mwl {

    void TimeSleep(const TimeSpan &sleepTime) {
#ifdef __MWL_WIN__
        Sleep(static_cast<DWORD>(sleepTime.ToU64(MILLISEC)));
#else
        uint64_t nanoSec = sleepTime.ToU64(NANOSEC);
        struct timespec request, remain;
        request.tv_sec = nanoSec / 1000000000;
        request.tv_nsec = nanoSec % 1000000000;
        int32_t ret = nanosleep(&request, &remain);
        while(ret < 0 && EINTR == errno) {
            request = remain;
            ret = nanosleep(&request, &remain);
        }
        if (ret < 0) {
            int32_t err = errno;
            MWL_WARN_ERRNO("nanosleep failed", err);
        }
#endif
    }

}
