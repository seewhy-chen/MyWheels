#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "BarrierImplement.h"

namespace mwl {

    Barrier::Implement::Implement(int32_t threadCount)
    : _threadCnt(threadCount) {
        //InitializeSynchronizationBarrier(&_b, threadCount, -1);
    }

    Barrier::Implement::~Implement() {
        //DeleteSynchronizationBarrier(&_b);
    }

    int32_t Barrier::Implement::_Wait() {
        return -1;
    }
}

#endif // __MWL_WIN__
