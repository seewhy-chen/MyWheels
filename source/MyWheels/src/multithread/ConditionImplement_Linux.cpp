#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "ConditionImplement.h"
#include "MutexImplement.h"

#include <sys/time.h>

namespace mwl {

    Condition::Implement::Implement() {
        int32_t ret = pthread_cond_init(&cond, nullptr);
        if (ret) {
            MWL_WARN_ERRNO("init cond failed", ret);
        }
    }

    Condition::Implement::~Implement() {
        int32_t ret = pthread_cond_destroy(&cond);
        if (ret) {
            MWL_WARN_ERRNO("destroy cond failed", ret);
        }
    }

    int32_t Condition::Implement::_Wait(Mutex &mutex, const TimeSpan *pTimeout) {
        int32_t timeoutInUs = pTimeout ? pTimeout->ToI32(MICROSEC) : -1;
        int32_t ret = ERR_NONE;
        if (timeoutInUs < 0) {
            ret = pthread_cond_wait(&cond, &mutex.Impl()->_m);
        } else {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            struct timespec ts;
            ts.tv_sec = tv.tv_sec + (tv.tv_usec + timeoutInUs) / 1000000;
            ts.tv_nsec = ((tv.tv_usec + timeoutInUs) % 1000000) * 1000;
            ret = pthread_cond_timedwait(&cond, &mutex.Impl()->_m, &ts);
        }

        if (ret) {
            if (ret != ETIMEDOUT) {
                MWL_WARN_ERRNO("wait cond failed", ret);
                ret = -ret;
            } else {
                ret = ERR_TIMEOUT;
            }
        }
        return ret;
    }

    int32_t Condition::Implement::_Signal() {
        int32_t ret = pthread_cond_signal(&cond);
        if (ret) {
            MWL_WARN_ERRNO("signal cond failed", ret);
        }
        return -ret;
    }

    int32_t Condition::Implement::_Broadcast() {
        int32_t ret = pthread_cond_broadcast(&cond);
        if (ret) {
            MWL_WARN_ERRNO("broadcast cond failed", ret);
        }
        return -ret;
    }
}

#endif //__MWL_LINUX__
