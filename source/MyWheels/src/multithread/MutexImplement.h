#ifndef __MWL_MUTEX_IMPLEMENT_H__
#define __MWL_MUTEX_IMPLEMENT_H__

#include "inc/Mutex.h"
#include "InternalCommon.h"

namespace mwl {

    struct Mutex::Implement {
        Implement(bool _traceOwner);
        ~Implement();
        int32_t _Lock();
        int32_t _TryLock();
        int32_t _Unlock();

#ifdef __MWL_WIN__
        SRWLOCK m;
#else
        pthread_mutex_t m;
#endif
        bool _traceOwner;
        ThreadID _owner;
    };

}

#endif // __MWL_MUTEX_IMPLEMENT_H__
