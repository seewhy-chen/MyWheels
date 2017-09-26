#include "inc/Thread.h"
#include "inc/Barrier.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

static int32_t BarrierTester(ThreadContext *pCtx) {
    ThreadID threadID = CurrentThreadID();
    MWL_INFO("%s started as (%lu, %lu), parent is (%lu, %lu)", 
        pCtx->Tag().C_Str(), threadID.pid, threadID.tid, pCtx->ParentID().pid, pCtx->ParentID().tid);


    MWL_INFO("%s stopped", pCtx->Tag().C_Str());
    return 0;
}

void TestBarrier() {
    MWL_INFO("TestBarrier started");
    Thread t1, t2;
    t1.SetTag("t1");
    t2.SetTag("t2");
    t1.Start(BarrierTester);
    t2.Start(BarrierTester);
    SharedPtr<Thread> t3 = StartThread(BarrierTester, nullptr, "t3");

    t1.Join();
    t2.Join();
    t3->Join();

    MWL_INFO("TestThread done\n");
}
