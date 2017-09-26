#ifndef __MWL_RWLOCK_IMPLEMENT_H__
#define __MWL_RWLOCK_IMPLEMENT_H__

#include "inc/RWLock.h"

#ifdef __MWL_LINUX__
#include <pthread.h>
#endif

namespace mwl {

    struct RWLock::Implement {
        Implement();
        ~Implement();
        int32_t _ReadLock();
        int32_t _TryReadLock();
        int32_t _ReadUnlock();
        int32_t _WriteLock();
        int32_t _TryWriteLock();
        int32_t _WriteUnlock();

    private:
#ifdef __MWL_WIN__
        SRWLOCK _l;
#elif defined __MWL_LINUX__
        pthread_rwlock_t _l;
#endif
    };
}

#endif // __MWL_RWLOCK_IMPLEMENT_H__
