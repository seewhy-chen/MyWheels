#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Semaphore.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

int32_t SemaphoreTester() {
    ThreadID threadID;
    GetCurrentThreadID(threadID);
    Semaphore sem;
    sem.Open("test", 0);
    MWL_INFO("(%lu, %lu) started", threadID.pid, threadID.tid);
    if (sem.Wait(&TimeSpec(1000)) == ERR_TIMEOUT) {
        MWL_INFO("(%lu, %lu) wait semaphore timeout", threadID.pid, threadID.tid);
    } else {
        MWL_INFO("(%lu, %lu) got semaphore, hold it 400ms then post it", threadID.pid, threadID.tid);
        TimeSleep(400);
        sem.Post();
    }
    MWL_INFO("(%lu, %lu) stopped", threadID.pid, threadID.tid);
    return 0;
}

void TestSemaphore() {
    MWL_INFO("TestSemaphore started");

    Semaphore sem;
    sem.Open("test", 0);

    Thread t1,t2,t3;
    t1.Start(SemaphoreTester);
    t2.Start(SemaphoreTester);
    t3.Start(SemaphoreTester);

    TimeSleep(500);
    MWL_INFO("posting...");
    sem.Post();

    t1.Join();
    t2.Join();
    t3.Join();

    MWL_INFO("TestSemaphore done\n");
}
