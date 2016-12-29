#include "BasicDefines.h"

#ifdef __MWL_WIN__

#include "ConditionImplement.h"
#include "MutexImplement.h"

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
                MWL_WARN_ERRNO("wait cond failed: %s(%d)", err);
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
