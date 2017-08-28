#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "SemaphoreImplement.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

namespace mwl {

    Semaphore::Implement::Implement() {
        s = nullptr;
        // FIXME: do we really need this?
        createdByMe = false;
    }

    Semaphore::Implement::~Implement() {
        _Close();
    }

    int32_t Semaphore::Implement::_Open(const String &name, int32_t initVal) {
        _Close();
        int32_t err = 0;
        s = sem_open(name.C_Str(), O_CREAT|O_EXCL|O_RDWR, S_IRWXU, initVal);
        if (!s) {
            err = errno;
            if (EEXIST == err) {
                s = sem_open(name.C_Str(), O_RDWR, S_IRWXU, initVal);
            }
        } else {
            createdByMe = true;
        }

        if (!s) {
            err = errno;
            MWL_WARN_ERRNO("create semaphore %s failed", err, name.C_Str());
        } else {
            this->name = name;
        }
        return s ? ERR_NONE : -err;
    }

    int32_t Semaphore::Implement::_Wait(const TimeSpan *pTimeout) {
        if (!s) {
            MWL_WARN("semaphore not opend when waiting");
            return ERR_INVAL_PARAM;
        }
        int32_t ret = 0;
        int32_t timeoutInUs = pTimeout ? pTimeout->ToI32(MICROSEC) : -1;
        if (timeoutInUs < 0) {
            ret = sem_wait(s);
        } else {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            struct timespec ts;
            ts.tv_sec = tv.tv_sec + (tv.tv_usec + timeoutInUs) / 1000000;
            ts.tv_nsec = ((tv.tv_usec + timeoutInUs) % 1000000) * 1000;
            ret = sem_timedwait(s, &ts);
        }

        if (ret < 0) {
            int32_t err = errno;
            if (err != ETIMEDOUT) {
                MWL_WARN_ERRNO("wait semaphore %s failed", ret, name.C_Str());
            }
            ret = -err;
        }
        return ret;
    }

    int32_t Semaphore::Implement::_Post(int32_t n) {
        if (!s) {
            MWL_WARN("semaphore not opend when posting");
            return ERR_INVAL_PARAM;
        }

        if (n <= 0) {
            MWL_WARN("semaphore can't been posted with n = %d", n);
            return ERR_INVAL_PARAM;
        }
    
        while (n-- > 0) {
            int32_t ret = sem_post(s);
            if (ret < 0) {
                int32_t err = errno;
                MWL_WARN_ERRNO("post semaphore %s failed", ret, name.C_Str());
                return -err;
            }
        }
        return 0;
    }
    
    int32_t Semaphore::Implement::_Close() {
        if (s) {
            sem_close(s);
            if (createdByMe) {
                sem_unlink(name.C_Str());
            }
            s = nullptr;
            name.Clear();
        }
        return ERR_NONE;
    }

}

#endif // __MWL_LINUX__
