#ifndef __MWL_RWLOCK_H__
#define __MWL_RWLOCK_H__

#include "inc/BasicDefines.h"

namespace mwl {

    class MWL_API RWLock : private NonCopyable {
    public:
        RWLock();
        ~RWLock();
        int32_t ReadLock();
        int32_t TryReadLock();
        int32_t ReadUnlock();
        int32_t WriteLock();
        int32_t TryWriteLock();
        int32_t WriteUnlock();

        struct MWL_API AutoReadLock {
            explicit AutoReadLock(RWLock &rwLock);
            ~AutoReadLock();
        private:
            RWLock &_rwLock;
        };

        struct MWL_API AutoWriteLock {
            explicit AutoWriteLock(RWLock &rwLock);
            ~AutoWriteLock();
        private:
            RWLock &_rwLock;
        };

    private:
        struct Implement;
        Implement *m_pImpl;
    };
}

#endif // __MWL_RWLOCK_H__
