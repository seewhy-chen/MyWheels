#include "inc/Thread.h"
#include "inc/Barrier.h"
#include "inc/Mutex.h"
#include "inc/TimeUtils.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct BarrierTestData {
    Mutex m;
    Barrier b;
    int32_t waitingCnt;
};

static int32_t BarrierTester(ThreadContext *pCtx) {
    BarrierTestData *pData = pCtx->SharedData<BarrierTestData>();
    pData->m.Lock();
    pData->waitingCnt += 1;
    pData->m.Unlock();
    TimeSleep(100);
    if (pData->b.Wait()) {
        pData->m.Lock();
        MWL_INFO("%s arrived lastly, %d is waiting", pCtx->Tag().C_Str(), pData->waitingCnt);
        MWL_ASSERT(pData->waitingCnt >= pData->b.Threshold());
        pData->waitingCnt -= pData->b.Threshold();
        pData->m.Unlock();
    }
    return 0;
}

void TestBarrier() {
    MWL_INFO("TestBarrier started");
    int32_t threadCnt = 100;
    BarrierTestData testData;
    testData.b.SetThreshold(threadCnt / 2);
    testData.waitingCnt = 0;
    Array<SharedPtr<Thread> > threads;
    for (int32_t i = 0; i < threadCnt; ++i) {
        threads.Append(SharedPtr<Thread>(new Thread()));
        threads.Back()->SetTag(String("").Format("thread %03d", i));
    }

    int32_t loopCnt = 50;
    for (int32_t j = 0; j < loopCnt; ++j) {
        for (int32_t i = 0; i < threadCnt; ++i) {
            threads[i]->Start(BarrierTester, &testData);
        }

        for (int32_t i = 0; i < threadCnt; ++i) {
            threads[i]->Join();
        }
    }

    MWL_INFO("TestBarrier done\n");
}
