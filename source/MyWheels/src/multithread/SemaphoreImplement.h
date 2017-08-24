#ifndef __MWL_SEMAPHORE_IMPLEMENT_H__
#define __MWL_SEMAPHORE_IMPLEMENT_H__

#include "inc/Semaphore.h"
#include "InternalCommon.h"

#ifdef __MWL_LINUX__
#include <semaphore.h>
#endif

#include <string>

namespace mwl {

    struct Semaphore::Implement {
        Implement();
        ~Implement();
        int32_t _Open(const String &name, int32_t initVal);
        int32_t _Wait(const TimeSpan *pTimeout);
        int32_t _Post(int32_t n);
        int32_t _Close();

#ifdef __MWL_WIN__
        HANDLE s;
#else
        sem_t* s;
#endif
        String name;
        bool createdByMe;

    };

}

#endif // __MWL_SEMAPHORE_IMPLEMENT_H__
