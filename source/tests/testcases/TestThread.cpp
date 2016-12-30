#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Condition.h"
#include "inc/Mutex.h"
#include "inc/Semaphore.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct MyThread : public Thread {
    MyThread(const char *tag) {
        SetTag(tag);
    }
    int32_t Entry() {
        MWL_INFO("%s started as (%u, %u), parent is (%u, %u)", 
            Tag(), Self().PID(), Self().TID(), Parent().PID(), Parent().TID());

        TimeSleep(500); 

        MWL_INFO("%s stopped", Tag());
        return 0;
    }
};

void TestThread() {
    MWL_INFO("TestThread started");

    ThreadID threadID;
    GetCurrentThreadID(threadID);

    MyThread t1("t1"), t2("t2"), t3("t3");
    MWL_INFO("(%u, %u) starting t1", threadID.PID(), threadID.TID());
    t1.Start();
    MWL_INFO("(%u, %u) starting t2", threadID.PID(), threadID.TID());
    t2.Start();
    MWL_INFO("(%u, %u) starting t3", threadID.PID(), threadID.TID());
    t3.Start();

    t1.Join();
    t2.Join();
    t3.Join();

    MWL_INFO("TestThread done\n");
}
