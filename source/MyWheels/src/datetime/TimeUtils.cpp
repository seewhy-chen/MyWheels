#include "TimeDefines.h"
#include "TimeCounterImplement.h"

#ifdef __MWL_WIN__
#else
#include <unistd.h> // for usleep
#endif

namespace mwl {
    void TimeSleep(double sleepTime, TimeUnit unit) {
        uint64_t microSec = 0;
        switch(unit) {
        case HOUR:
            microSec = static_cast<uint64_t>(sleepTime * 3600 * 1000000);
            break;
        case MINUTE:
            microSec = static_cast<uint64_t>(sleepTime * 60 * 1000000);
            break;
        case SECOND:
            microSec = static_cast<uint64_t>(sleepTime * 1000000);
            break;
        default:
            MWL_ERR("unknown time unit %d", unit);
            // fall through
        case MILLISEC:
            microSec = static_cast<uint64_t>(sleepTime * 1000);
            break;
        case MICROSEC:
            microSec = static_cast<uint64_t>(sleepTime);
            break;
            
        }
#ifdef __MWL_WIN__
        Sleep(static_cast<DWORD>(microSec/1000));
#else
        usleep(microSec);
#endif
    }
}
