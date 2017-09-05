#ifndef __MWL_SEMAPHORE_H__
#define __MWL_SEMAPHORE_H__

#include "inc/BasicDefines.h"
#include "inc/String.h"
#include "inc/TimeSpan.h"

namespace mwl {

    class MWL_API Semaphore : private NonCopyable {
    public:
        Semaphore();
        ~Semaphore();
        int32_t Open(const String &name, int32_t initVal = 1);
        int32_t Wait(const TimeSpan *pTimeout = nullptr);
        int32_t Post(int32_t n = 1);
        int32_t Close();

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_SEMAPHORE_H__
