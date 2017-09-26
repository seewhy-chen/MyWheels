#ifndef __MWL_BARRIER_IMPLEMENT_H__
#define __MWL_BARRIER_IMPLEMENT_H__

#include "inc/Barrier.h"

#ifdef __MWL_LINUX__
#include <pthread.h>
#endif

namespace mwl {
    struct Barrier::Implement {
        Implement(int32_t threadCount);
        ~Implement();
        int32_t _Wait();

    private:

#ifdef __MWL_WIN__
        SYNCHRONIZATION_BARRIER _b;
#elif defined __MWL_LINUX__
        pthread_barrier_t _b;
#endif

        int32_t _threadCnt;
    };
}

#endif // __MWL_BARRIER_H__
