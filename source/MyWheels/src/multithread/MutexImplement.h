#ifndef __MWL_MUTEX_IMPLEMENT_H__
#define __MWL_MUTEX_IMPLEMENT_H__

#include "Mutex.h"
#include "InternalCommon.h"

namespace mwl {
    struct Mutex::Implement {
        Implement();
        ~Implement();
        int32_t _Lock();
        int32_t _TryLock();
        int32_t _Unlock();

    private:
#ifdef __WINDOWS__
        CRITICAL_SECTION _m;
#else
        pthread_mutex_t _m;
#endif
    };
}

#endif // __MWL_MUTEX_IMPLEMENT_H__
