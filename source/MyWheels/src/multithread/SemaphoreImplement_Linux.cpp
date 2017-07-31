#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "SemaphoreImplement.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

namespace mwl {

    Semaphore::Implement::Implement() {
        s = nullptr;
    }

    Semaphore::Implement::~Implement() {
        _Close();
    }

    int32_t Semaphore::Implement::_Open(const char *name, int32_t initVal) {
        _Close();
        int32_t err = 0;
        s = sem_open(name, O_CREAT|O_EXCL|O_RDWR, S_IRWXU, initVal);
        if (!s) {
            err = errno;
            if (EEXIST == err) {
                s = sem_open(name, O_RDWR, S_IRWXU, initVal);
            }
        } else {
            createdByMe = true;
        }

        if (!s) {
            err = errno;
            MWL_WARN_ERRNO("create semaphore %s failed", err, name);
        } else {
            this->name = name;
        }
        return s ? ERR_NONE : -err;
    }

    int32_t Semaphore::Implement::_Wait(const TimeSpec *pTimeout) {
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
                MWL_WARN_ERRNO("wait semaphore %s failed", ret, name.c_str());
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
                MWL_WARN_ERRNO("post semaphore %s failed", ret, name.c_str());
                return -err;
            }
        }
        return 0;
    }
    
    int32_t Semaphore::Implement::_Close() {
        if (s) {
            sem_close(s);
            if (createdByMe) {
                sem_unlink(name.c_str());
            }
            s = nullptr;
            name.clear();
        }
        return ERR_NONE;
    }

}

#endif // __MWL_LINUX__
