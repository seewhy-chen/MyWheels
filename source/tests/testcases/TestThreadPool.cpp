#include "inc/ThreadPool.h"
#include "inc/Randomizer.h"
#include "inc/TimeUtils.h"
#include "inc/Mutex.h"
#include "inc/Condition.h"
#include "InternalCommon.h"
using namespace mwl;

struct ThreadPoolTestData{
    int32_t sleepTime;
};

MWL_THREAD_ENTRY(ThreadPoolTester, pContext) {
    ThreadPoolTestData *pData = pContext->SharedData<ThreadPoolTestData>();
    MWL_INFO("[%lu-%lu] sleep %d ms", pContext->SelfID().pid, pContext->SelfID().tid, pData->sleepTime);
    TimeSleep(pData->sleepTime);
    MWL_INFO("[%lu-%lu] exited", pContext->SelfID().pid, pContext->SelfID().tid);
    return 0;
}

struct _ThreadPoolListener : public ThreadPoolListener {
    Mutex m;
    Condition c;
    int32_t OnThreadPoolEvent(ThreadPoolEvent event, void *) {
        if (TPEVT_ALL_DONE == event) {
            c.Signal();
        }
        return 0;
    }
};

void TestThreadPool() {
    Randomizer rnd;
    int32_t taskCnt = 100;
    int32_t maxThreadCnt = 5;
    Array<ThreadPoolTestData*> testData;
    ThreadPool pool;
    _ThreadPoolListener listener;
    listener.m.Lock();
    pool.RegisterListener(&listener);
    pool.SetMaxThreadCount(maxThreadCnt);
    for (int32_t i = 0; i < taskCnt; ++i) {
        testData.Append(new ThreadPoolTestData());
        testData.Back()->sleepTime = rnd.NextS32InRange(100, 1000);
        pool.AddTask(ThreadPoolTester, testData.Back());
    }
    TimeSleep(1000);
    pool.SetMaxThreadCount(pool.GetMaxThreadCount() + maxThreadCnt);
    if (listener.c.Wait(listener.m, 100) < 0) {
        pool.SetMaxThreadCount(pool.GetMaxThreadCount() + maxThreadCnt);
    }
    TimeSleep(1000);
    pool.Cancel();
    listener.m.Unlock();

    for (int32_t i = 0; i < testData.Size(); ++i) {
        delete testData[i];
    }
}
