#ifndef __MWL_BARRIER_IMPLEMENT_H__
#define __MWL_BARRIER_IMPLEMENT_H__

#include "inc/Barrier.h"

#ifdef __MWL_WIN__
#include "inc/Mutex.h"
#include "inc/Condition.h"
#elif defined __MWL_LINUX__
#include <pthread.h>
#endif

namespace mwl {
    struct Barrier::Implement {
        Implement(int32_t threshold);
        ~Implement();
        int32_t _Wait();
        int32_t _SetThreshold(int32_t threshold);
        int32_t _Threshold();
    private:

#ifdef __MWL_WIN__
        Mutex _m;
        Condition _c;
        int32_t _switch;
        int32_t _waitingCnt;
#elif defined __MWL_LINUX__
        pthread_barrier_t _b;
#endif

        int32_t _threshold;
    };
}

#endif // __MWL_BARRIER_IMPLEMENT_H__
