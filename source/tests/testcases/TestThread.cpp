#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/TimeUtils.h"
#include "inc/Condition.h"
#include "inc/Mutex.h"
#include "inc/Semaphore.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

static int32_t ThreadTester(ThreadContext *pCtx) {
    ThreadID threadID = CurrentThreadID();
    MWL_INFO("%s started as (%lu, %lu), parent is (%lu, %lu)", 
        pCtx->Tag().C_Str(), threadID.pid, threadID.tid, pCtx->ParentID().pid, pCtx->ParentID().tid);

    TimeSleep(500); 

    MWL_INFO("%s stopped", pCtx->Tag().C_Str());
    return 0;
}

void TestThread() {
    MWL_INFO("TestThread started");
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
