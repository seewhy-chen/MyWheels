#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "SemaphoreImplement.h"

namespace mwl {

    Semaphore::Implement::Implement() {
        _s = nullptr;
        createdByMe = false;
    }

    Semaphore::Implement::~Implement() {
        _Close();
    }

    int32_t Semaphore::Implement::_Open(const String &name, int32_t initVal) {
        _Close();
        _s = CreateSemaphore(nullptr, initVal, 0x7FFFFFFFL, (name.Empty() ? nullptr : name.C_Str()));
        int32_t err = GetLastError();
        if (!_s) {
            MWL_WARN_ERRNO("create semaphore %s failed", err, name.C_Str());
        } else {
            _name = name;
            createdByMe = (err == ERROR_SUCCESS);
        }
        return _s ? ERR_NONE : -err;
    }

    int32_t Semaphore::Implement::_Wait(const TimeSpan *pTimeout) {
        if (!_s) {
            MWL_WARN("semaphore not opend when waiting");
            return ERR_INVAL_PARAM;
        }
        int32_t timeoutInMs = INFINITE;
        if (pTimeout) {
            timeoutInMs = pTimeout->ToI32(MILLISEC);
            if (timeoutInMs < 0) {
                timeoutInMs = INFINITE;
            }
        }

        int32_t ret = WaitForSingleObject(_s, timeoutInMs);
        switch (ret) {
        case WAIT_OBJECT_0: {
                ret = ERR_NONE;
            }
            break;

        case WAIT_TIMEOUT: {
                ret = ERR_TIMEOUT;
            }
            break;

        case WAIT_FAILED: {
                int32_t err = GetLastError();
                MWL_WARN_ERRNO("wait semaphore %s failed", err, _name.C_Str());
                ret = -err;
            }
            break;

        default: {
                int32_t err = GetLastError();
                MWL_WARN_ERRNO("wait semaphore %s got wait result %d", err, _name.C_Str(), ret);
                if (err != 0) {
                    ret = -err;
                } else {
                    ret = -ret;
                }
            }
            break;
        }

        return ret;
    }

    int32_t Semaphore::Implement::_Post(int32_t n) {
        if (!_s) {
            MWL_WARN("semaphore not opend when posting");
            return ERR_INVAL_PARAM;
        }

        if (!ReleaseSemaphore(_s, n, nullptr)) {
            int32_t err = GetLastError();
            MWL_WARN_ERRNO("release semaphore %s with n = %d failed", err, _name.C_Str(), n);
            return -err;
        }
        return ERR_NONE;
    }

    int32_t Semaphore::Implement::_Close() {
        if (_s && createdByMe) {
            CloseHandle(_s);
            _s = nullptr;
        }
        _name.Clear();
        return ERR_NONE;
    }
}

#endif // __MWL_WIN__
