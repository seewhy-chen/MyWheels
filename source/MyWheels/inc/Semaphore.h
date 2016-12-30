#ifndef __MWL_SEMAPHORE_H__
#define __MWL_SEMAPHORE_H__

#include "inc/BasicDefines.h"

namespace mwl {

    class MWL_API Semaphore : private NonCopyable {
    public:
        Semaphore();
        ~Semaphore();
        int32_t Open(const char *name, int32_t initVal);
        int32_t Wait(int32_t timeoutInMs = -1);
        int32_t Post();
        int32_t Close();

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_SEMAPHORE_H__
