#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "SemaphoreImplement.h"

namespace mwl {

    Semaphore::Implement::Implement() {
        s = nullptr;
        createdByMe = false;
    }

    Semaphore::Implement::~Implement() {
        _Close();
    }

    int32_t Semaphore::Implement::_Open(const char *name, int32_t initVal) {
        _Close();
        s = CreateSemaphore(nullptr, initVal, 0x7FFFFFFFL, name);
        int32_t err = GetLastError();
        if (!s) {
            MWL_WARN_ERRNO("create semaphore %s failed", err, name);
        } else {
            this->name = name ? name : "";
            createdByMe = (err == ERROR_SUCCESS);
        }
        return s ? ERR_NONE : -err;
    }

    int32_t Semaphore::Implement::_Wait(int32_t timeoutInMs) {
        if (!s) {
            return ERR_INVAL_PARAM;
        }

        if (timeoutInMs < 0) {
            timeoutInMs = INFINITE;
        }

        int32_t ret = WaitForSingleObject(s, timeoutInMs);
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
                MWL_WARN_ERRNO("wait semaphore %s failed", err, name.c_str());
                ret = -err;
            }
            break;

        default: {
                int32_t err = GetLastError();
                MWL_WARN_ERRNO("wait semaphore %s got wait result %d", err, name.c_str(), ret);
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

    int32_t Semaphore::Implement::_Post() {
        if (!s) {
            return ERR_INVAL_PARAM;
        }

        if (!ReleaseSemaphore(s, 1, nullptr)) {
            int32_t err = GetLastError();
            MWL_WARN_ERRNO("release semaphore %s failed", err, name.c_str());
            return -err;
        }
        return ERR_NONE;
    }

    int32_t Semaphore::Implement::_Close() {
        if (s && createdByMe) {
            CloseHandle(s);
        }
        s = nullptr;
        name.clear();
        return ERR_NONE;
    }
}

#endif // __MWL_WIN__
