#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "SemaphoreImplement.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

namespace mwl {

    Semaphore::Implement::Implement() {
        _pS = nullptr;
        createdByMe = false;
    }

    Semaphore::Implement::~Implement() {
        _Close();
    }

    int32_t Semaphore::Implement::_Open(const String &name, int32_t initVal) {
        int32_t ret = _Close();
        if (ret < 0) {
            return ret;
        }
        if (name.Empty()) {
            ret = sem_init(&_s, 0, initVal);
            if (ret < 0) {
                ret = errno;
                MWL_WARN_ERRNO("sem_init failed", ret);
            } else {
                _pS = &_s;
                createdByMe = true;
            }
        } else {
            _pS = sem_open(name.C_Str(), O_CREAT | O_EXCL | O_RDWR, S_IRWXU, initVal);
            if (!_pS) {
                if (EEXIST == errno) {
                    _pS = sem_open(name.C_Str(), O_RDWR, S_IRWXU, initVal);
                }
            } else {
                createdByMe = true;
            }
            if (_pS) {
                _name = name;
            } else {
                ret = errno;
                MWL_WARN_ERRNO("sem_open %s failed", ret, name.C_Str());
            }
        }

        return -ret;
    }

    int32_t Semaphore::Implement::_Wait(const TimeSpan *pTimeout) {
        if (!_pS) {
            MWL_WARN("semaphore not opend when waiting");
            return ERR_INVAL_PARAM;
        }
        int32_t ret = 0;
        int32_t timeoutInUs = pTimeout ? pTimeout->ToI32(MICROSEC) : -1;
        if (timeoutInUs < 0) {
            ret = sem_wait(_pS);
        } else {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            struct timespec ts;
            ts.tv_sec = tv.tv_sec + (tv.tv_usec + timeoutInUs) / 1000000;
            ts.tv_nsec = ((tv.tv_usec + timeoutInUs) % 1000000) * 1000;
            ret = sem_timedwait(_pS, &ts);
        }

        if (ret < 0) {
            ret = errno;
            if (ret != ETIMEDOUT) {
                MWL_WARN_ERRNO("wait semaphore %s failed", ret, _name.C_Str());
            }
        }
        return -ret;
    }

    int32_t Semaphore::Implement::_Post(int32_t n) {
        if (!_pS) {
            MWL_WARN("semaphore not opend when posting");
            return ERR_INVAL_PARAM;
        }
        if (n <= 0) {
            MWL_WARN("semaphore can't been posted with n = %d", n);
            return ERR_INVAL_PARAM;
        }

        while (n-- > 0) {
            int32_t ret = sem_post(_pS);
            if (ret < 0) {
                ret = errno;
                MWL_WARN_ERRNO("post semaphore %s failed", ret, _name.C_Str());
                return -ret;
            }
        }
        return ERR_NONE;
    }

    int32_t Semaphore::Implement::_Close() {
        if (!_pS) {
            return ERR_NONE;
        }
        int32_t ret = sem_close(_pS);
        if (ret < 0) {
            ret = errno;
            MWL_WARN_ERRNO("sem_close failed", ret);
        } else {
            _pS = nullptr;
            if (createdByMe && !_name.Empty()) {
                int32_t r = sem_unlink(_name.C_Str());
                if (r < 0) {
                    MWL_WARN_ERRNO("sem_unlink %s failed", r, _name.C_Str());
                }
            }
            _name.Clear();
        }
        return -ret;
    }
}

#endif // __MWL_LINUX__
