#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Condition.h"
#include "inc/Mutex.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct ConditionTest {
    Condition &cond;
    Mutex &mutex;
    ConditionTest(Condition &cond_, Mutex &mutex_) : cond(cond_), mutex(mutex_) {
    }
};

int32_t Entry(ThreadContext *pCtx) {
    ConditionTest *pTest = reinterpret_cast<ConditionTest*>(pCtx->SharedData());
    MWL_INFO("%s started", pCtx->Tag());
    TimeSpec timeout(1000);
    Mutex::AutoLock _l(pTest->mutex);
    if (pTest->cond.Wait(pTest->mutex, &timeout) == ERR_TIMEOUT) {
        MWL_INFO("%s wait cond timeout", pCtx->Tag()); 
    }
    MWL_INFO("%s stopped", pCtx->Tag());
    return 0;
}


void TestCondition() {
    MWL_INFO("TestCondition started");

    Condition cond;
    Mutex mutex;
    ConditionTest t1(cond, mutex), t2(cond, mutex), t3(cond, mutex);
    Thread thread1, thread2, thread3;
    thread1.SetTag("t1");
    thread2.SetTag("t2");
    thread3.SetTag("t3");
    thread1.Start(Entry, &t1);
    thread2.Start(Entry, &t2);
    thread3.Start(Entry, &t3);

    TimeSleep(500);
    cond.Signal();
    MWL_INFO("signaled");

    TimeSleep(200);
    cond.Broadcast();
    MWL_INFO("broadcasted");

    thread1.Join();
    thread2.Join();
    thread3.Join();

    MWL_INFO("TestCondition done\n");
}
