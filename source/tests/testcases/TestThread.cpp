#include "Thread.h"
#include "TimeDefines.h"
#include "InternalCommon.h"

using namespace mwl;

#include <string>

struct MyThread : public Thread {
    MyThread(const char *tag = "") {
        SetTag(tag);
    }
    int32_t Entry() {
        ThreadStarted();

        MWL_INFO("(%u, %u) %s started, parent is (%u, %u)", 
            Self().PID(), Self().TID(), Tag(), Parent().PID(), Parent().TID());
        TimeSleep(1000);
        MWL_INFO("(%u, %u) %s stopped", Self().PID(), Self().TID(), Tag());

        return 0;
    }
};

void TestThread() {
    MyThread t1, t2("t2"), t3("t3");
    t1.Start();
    t2.Start();
    t3.Start();
    t1.Join();
    t2.Join();
    t3.Join();
}
