#include "inc/Thread.h"
#include "inc/RWLock.h"
#include "inc/TimeUtils.h"
#include "inc/Mutex.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct RWLockTesterData {
    int32_t wrCnt;
    int32_t rdCnt;
    int32_t maxRdCnt;
    Mutex m;
    RWLock rwLck;
};

static MWL_THREAD_ENTRY(RWLockTester, pCtx) {
    RWLockTesterData *pData = pCtx->SharedData<RWLockTesterData>();
    if (pCtx->Tag() == "reader") {
        pData->rwLck.ReadLock();
        MWL_ASSERT(pData->wrCnt == 0);
        pData->m.Lock();
        ++pData->rdCnt;
        if (pData->rdCnt > pData->maxRdCnt) {
            pData->maxRdCnt = pData->rdCnt;
        }
        pData->m.Unlock();

        TimeSleep(1);

        pData->m.Lock();
        --pData->rdCnt;
        pData->m.Unlock();
        pData->rwLck.ReadUnlock();
    } else {
        pData->rwLck.WriteLock();
        MWL_ASSERT(pData->rdCnt == 0);
        ++pData->wrCnt;
        TimeSleep(1000);
        --pData->wrCnt;
        pData->rwLck.WriteUnlock();
    }
    return 0;
}

void TestRWLock() {
    MWL_INFO("TestRWLock started");
    RWLock lck;
    RWLockTesterData testData;
    testData.wrCnt = 0;
    testData.rdCnt = 0;
    testData.maxRdCnt = 0;

    Array<SharedPtr<Thread> > threads;
    int32_t readerCnt = 200;
    threads.Append(SharedPtr<Thread>(new Thread("writer")));
    for (int32_t i = 0; i < readerCnt/2; ++i) {
        threads.Append(SharedPtr<Thread>(new Thread("reader")));
    }
    threads.Append(SharedPtr<Thread>(new Thread("writer")));
    for (int32_t i = 0; i < readerCnt/2; ++i) {
        threads.Append(SharedPtr<Thread>(new Thread("reader")));
    }
    threads.Append(SharedPtr<Thread>(new Thread("writer")));
    
    for (int32_t j = 0; j < 10; ++j) {
        for (int32_t i = 0; i < threads.Size(); ++i) {
            threads[i]->Start(RWLockTester, &testData);
        }

        for (int32_t i = 0; i < threads.Size(); ++i) {
            threads[i]->Join();
        }
        MWL_INFO("loop %d ended, maxRdCnt = %d", j, testData.maxRdCnt);
        testData.wrCnt = 0;
        testData.rdCnt = 0;
        testData.maxRdCnt = 0;
    }

    MWL_INFO("TestRWLock done");
}
