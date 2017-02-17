#ifndef __MWL_CONDITION_H__
#define __MWL_CONDITION_H__

#include "inc/BasicDefines.h"
#include "inc/TimeDefines.h"

namespace mwl {

    class Mutex;

    class MWL_API Condition : private NonCopyable {
    public:
        Condition();
        ~Condition();
        int32_t Wait(Mutex &mutex, const TimeSpec *pTimeout = nullptr);
        int32_t Signal();
        int32_t Broadcast();

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_CONDITION_H__
