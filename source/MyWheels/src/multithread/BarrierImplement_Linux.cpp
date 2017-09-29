#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "BarrierImplement.h"

namespace mwl {

    Barrier::Implement::Implement(int32_t threshold) {
        int32_t ret = pthread_barrier_init(&_b, nullptr, threshold);
        if (ret) {
            MWL_WARN_ERRNO("init barrier failed", ret);
        } else {
            _threshold = threshold;
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

    int32_t Barrier::Implement::_SetThreshold(int32_t threshold) {
        if (threshold != _threshold) {
            int32_t ret = pthread_barrier_destroy(&_b);
            if (ret) {
                MWL_WARN_ERRNO("destroy barrier failed", ret);
                return -ret;
            }

            ret = pthread_barrier_init(&_b, nullptr, threshold);
            if (ret) {
                MWL_WARN_ERRNO("init barrier failed", ret);
                return ret;
            }
            _threshold = threshold;
        }
        return _threshold;
    }
    int32_t Barrier::Implement::_Threshold() {
        return _threshold;
    }
}

#endif // __MWL_LINUX__
