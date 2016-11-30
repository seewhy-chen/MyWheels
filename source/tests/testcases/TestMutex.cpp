#include "Mutex.h"

#include "InternalCommon.h"

using namespace mwl;

void TestMutex() {
    Mutex m;
    int32_t ret = m.Lock();
    fprintf(stdout, "m.Lock() returned %d\n", ret);
    if (ret != 0) {
        return;
    }
    ret = m.TryLock();
    fprintf(stdout, "m.TryLock() returned %d\n", ret);

    ret = m.Unlock();
    fprintf(stdout, "m.Unlock() returned %d\n", ret);
}
