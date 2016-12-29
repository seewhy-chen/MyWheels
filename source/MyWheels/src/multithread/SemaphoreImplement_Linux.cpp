#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "SemaphoreImplement.h"

namespace mwl {

    Semaphore::Implement::Implement() {
        s = sem_open("test", 0);
    }

    Semaphore::Implement::~Implement() {

    }

    int32_t Semaphore::Implement::_Open(const char *name, int32_t initVal) {
        return -1;
    }

    int32_t Semaphore::Implement::_Wait(int32_t timeoutInMs) {
        return -1;
    }

    int32_t Semaphore::Implement::_Post() {
        return -1;
    }
    
    int32_t Semaphore::Implement::_Close() {
        return -1;
    }

}

#endif // __MWL_LINUX__
