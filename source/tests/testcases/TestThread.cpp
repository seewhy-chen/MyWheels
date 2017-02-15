#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Condition.h"
#include "inc/Mutex.h"
#include "inc/Semaphore.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

int32_t ThreadTester(ThreadContext *pCtx) {
    ThreadID threadID;
    GetCurrentThreadID(threadID);
    MWL_INFO("%s started as (%lu, %lu), parent is (%lu, %lu)", 
        pCtx->Tag(), threadID.pid, threadID.tid, pCtx->ParentID().pid, pCtx->ParentID().tid);

    TimeSleep(500); 

    MWL_INFO("%s stopped", pCtx->Tag());
    return 0;
}

void TestThread() {
    MWL_INFO("TestThread started");

    ThreadID threadID;
    GetCurrentThreadID(threadID);

    Thread t1, t2;
    t1.SetTag("t1");
    t2.SetTag("t2");
    t1.Start(ThreadTester);
    t2.Start(ThreadTester);
    SharedPtr<Thread> t3 = StartThread(ThreadTester, nullptr, "t3");

    t1.Join();
    t2.Join();
    t3->Join();

    MWL_INFO("TestThread done\n");
}
