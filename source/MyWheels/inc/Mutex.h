#include "BasicDefines.h"

namespace mwl {
    class MWL_API Mutex {
    public:
        Mutex();
        ~Mutex();
        int32_t Lock();
        int32_t TryLock();
        int32_t Unlock();

    private:
        struct Implement *m_pImpl;
    };
}
