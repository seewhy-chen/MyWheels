#ifndef __MWL_CONDITION_H__
#define __MWL_CONDITION_H__

#include "inc/BasicDefines.h"
#include "inc/TimeSpan.h"

namespace mwl {

    class Mutex;

    class MWL_API Condition : private NonCopyable {
    public:
        Condition();
        ~Condition();
        int32_t Wait(Mutex &mutex);
        int32_t Wait(Mutex &mutex, const TimeSpan &timeout);
        int32_t Signal();
        int32_t Broadcast();

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_CONDITION_H__
