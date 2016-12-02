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

    private:
        struct Implement;
        Implement *m_pImpl;
    };
}

#endif // __MWL_MUTEX_H__
