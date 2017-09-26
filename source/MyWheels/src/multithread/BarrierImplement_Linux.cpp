#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "BarrierImplement.h"

namespace mwl {

    Barrier::Implement::Implement(int32_t threadCount) {
        int32_t ret = pthread_barrier_init(&_b, nullptr, threadCount);
        if (ret) {
            MWL_WARN_ERRNO("init barrier failed", ret);
        }
    }

    Barrier::Implement::~Implement() {
        int32_t ret = pthread_barrier_destroy(&_b);
        if (ret) {
            MWL_WARN_ERRNO("destroy barrier failed", ret);
        }
    }

    int32_t Barrier::Implement::_Wait() {
        int32_t ret = pthread_barrier_wait(&_b);
        if (PTHREAD_BARRIER_SERIAL_THREAD == ret) {
            return 1;
        } else if (ret) {
            MWL_WARN_ERRNO("wait barrier failed", ret);
        }
        return -ret;
    }
}

#endif // __MWL_LINUX__
