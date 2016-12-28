#include "ConditionImplement.h"
#include "MutexImplement.h"

#ifdef __MWL_WIN__

namespace mwl {
    Condition::Implement::Implement() {
        InitializeConditionVariable(&cond);    
    }

    Condition::Implement::~Implement() {}

    int32_t Condition::Implement::_Wait(Mutex &mutex, int32_t timeoutInMs) {
        if (timeoutInMs < 0) {
            timeoutInMs = INFINITE;
        }
        if (!SleepConditionVariableCS(&cond, &mutex.Impl()->_m, timeoutInMs)) {
            int32_t err = GetLastError();
            if (err == ERROR_TIMEOUT) {
                return ERR_TIMEOUT;
            } else {
                char errMsg[512] = {0};
                strerror_s(errMsg, sizeof(errMsg), err);
                MWL_WARN("wait cond failed: %s(%d)", errMsg, err);
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
