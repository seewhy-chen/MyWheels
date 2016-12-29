#include "inc/Mutex.h"

#include "InternalCommon.h"

using namespace mwl;

void TestMutex() {
    Mutex m;
    int32_t ret = 0;

    ret = m.Unlock();
    if (ret != 0) {
        MWL_WARN("unlock failed with ret = %d", ret);
    }

    ret = m.Lock();
    if (ret != 0) {
        MWL_WARN("lock failed with ret = %d", ret);
        return;
    }

    ret = m.TryLock();
    if (ret != 0) {
        MWL_WARN("try lock failed with ret = %d", ret);
    }

    ret = m.Unlock();
    if (ret != 0) {
        MWL_WARN("unlock failed with ret = %d", ret);
    }
}
