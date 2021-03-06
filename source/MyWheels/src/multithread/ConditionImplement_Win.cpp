#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "ConditionImplement.h"
#include "MutexImplement.h"

namespace mwl {

    Condition::Implement::Implement() {
        InitializeConditionVariable(&cond);
    }

    Condition::Implement::~Implement() {}

    int32_t Condition::Implement::_Wait(Mutex &mutex, const TimeSpan *pTimeout) {
        int32_t timeoutInMs = INFINITE;
        if (pTimeout) {
            timeoutInMs = pTimeout->ToI32(MILLISEC);
            if (timeoutInMs < 0) {
                timeoutInMs = INFINITE;
            }
        }
        if (!SleepConditionVariableSRW(&cond, &mutex.Impl()->_m, timeoutInMs, 0)) {
            int32_t err = GetLastError();
            if (err == ERROR_TIMEOUT) {
                return ERR_TIMEOUT;
            } else {
                MWL_WARN_ERRNO("wait cond failed", err);
                return -err;
            }
        } else {
            return ERR_NONE;
        }
    }

    int32_t Condition::Implement::_Signal() {
        WakeConditionVariable(&cond);
        return 0;
    }

    int32_t Condition::Implement::_Broadcast() {
        WakeAllConditionVariable(&cond);
        return 0;
    }
}

#endif //__MWL_WIN__
