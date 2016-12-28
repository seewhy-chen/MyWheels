#ifndef __MWL_MUTEX_H__
#define __MWL_MUTEX_H__

#include "BasicDefines.h"

namespace mwl {
    class MWL_API Mutex : private NonCopyable {
    public:
        Mutex();
        ~Mutex();
        int32_t Lock();
        int32_t TryLock();
        int32_t Unlock();

        struct AutoLock {
            explicit AutoLock(Mutex &mutex);
            ~AutoLock();

        private:
            Mutex &_mutex;
        };

        struct Implement;
        Implement* Impl();
    private:
        Implement *m_pImpl;
    };
}

#endif // __MWL_MUTEX_H__
