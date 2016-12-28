#ifndef __MWL_CONDITION_H__
#define __MWL_CONDITION_H__

#include "BasicDefines.h"

namespace mwl {

    class Mutex;

    class MWL_API Condition : private NonCopyable {
    public:
        Condition();
        ~Condition();
        int32_t Wait(Mutex &mutex, int32_t timeoutInMs = -1);
        int32_t Signal();
        int32_t Broadcast();

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_CONDITION_H__
