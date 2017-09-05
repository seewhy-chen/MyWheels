#include "inc/Thread.h"
#include "inc/Mutex.h"
#include "inc/TimeUtils.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

int32_t MutexTester(ThreadContext *pCtx) {
    Mutex *pMutex = reinterpret_cast<Mutex*>(pCtx->SharedData());
    MWL_INFO("%s started", pCtx->Tag().C_Str());
    TimeSleep(500);
    pMutex->Lock();
    MWL_INFO("%s got the mutex", pCtx->Tag().C_Str());
    TimeSleep(500);
    pMutex->Unlock();
    MWL_INFO("%s stopped", pCtx->Tag().C_Str());
    return 0;
}

void TestMutex() {
    MWL_INFO("TestMutex started");

    Mutex mutex;
    MWL_ASSERT(mutex.Lock() >= 0);
    MWL_ASSERT(mutex.TryLock() < 0);
    MWL_ASSERT(mutex.Unlock() >= 0);
    Thread t1, t2, t3;
    t1.SetTag("t1");
    t2.SetTag("t2");
    t3.SetTag("t3");

    t1.Start(MutexTester, &mutex);
    t2.Start(MutexTester, &mutex);
    t3.Start(MutexTester, &mutex);

    t1.Join();
    t2.Join();
    t3.Join();

    MWL_INFO("TestMutex done\n");
}
