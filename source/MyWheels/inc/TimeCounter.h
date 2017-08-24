#ifndef __MWL_TIME_COUNTER_H__
#define __MWL_TIME_COUNTER_H__

#include "inc/BasicDefines.h"
#include "inc/String.h"
#include "TimeDefines.h"

namespace mwl {

    class MWL_API TimeCounter : private NonCopyable {
    public:
        TimeCounter();
        ~TimeCounter();
        int32_t Start(const String &tag = String::Null());
        uint64_t TimeElapsed(TimeUnit unit = MILLISEC);
        uint64_t TimeElapsed(const String &tag, TimeUnit unit = MILLISEC);
    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_TIME_COUNTER_H__
