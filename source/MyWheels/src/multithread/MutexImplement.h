#ifndef __MWL_MUTEX_IMPLEMENT_H__
#define __MWL_MUTEX_IMPLEMENT_H__

#include "InternalCommon.h"
#include "Mutex.h"

namespace mwl {

    struct Mutex::Implement {
        Implement();
        ~Implement();
        int32_t _Lock();
        int32_t _TryLock();
        int32_t _Unlock();

#ifdef __MWL_WIN__
        CRITICAL_SECTION m;
#else
        pthread_mutex_t m;
#endif
    };

}

#endif // __MWL_MUTEX_IMPLEMENT_H__
