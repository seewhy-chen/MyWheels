#ifndef __MWL_CONDITION_IMPLEMENT_H__
#define __MWL_CONDITION_IMPLEMENT_H__

#include "InternalCommon.h"

#include "Condition.h"

namespace mwl {
    struct Condition::Implement{
        Implement();
        ~Implement();
        int32_t _Wait(Mutex &mutex, int32_t timeoutInMs);
        int32_t _Signal();
        int32_t _Broadcast();
#ifdef __MWL_WIN__
        CONDITION_VARIABLE cond;
#else
#endif
    };
}

#endif // __MWL_CONDITION_IMPLEMENT_H__
