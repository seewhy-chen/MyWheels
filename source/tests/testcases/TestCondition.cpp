#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Condition.h"
#include "inc/Mutex.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct ConditionThread : public Thread {
    Condition &cond;
    Mutex &mutex;
    ConditionThread(const char *tag, Condition &cond_, Mutex &mutex_) : cond(cond_), mutex(mutex_) {
        SetTag(tag);
    }
    int32_t Entry() {
        MWL_INFO("%s started", Tag());
        Mutex::AutoLock _l(mutex);
        if (cond.Wait(mutex, 1000) == ERR_TIMEOUT) {
            MWL_INFO("%s wait cond timeout", Tag()); 
        }
        MWL_INFO("%s stopped", Tag());
        return 0;
    }
};

void TestCondition() {
    MWL_INFO("TestCondition started");

    Condition cond;
    Mutex mutex;
    ConditionThread t1("t1", cond, mutex), t2("t2", cond, mutex), t3("t3", cond, mutex);
    t1.Start();
    t2.Start();
    t3.Start();

    TimeSleep(500);
    cond.Signal();
    MWL_INFO("signaled");

    TimeSleep(200);
    cond.Broadcast();
    MWL_INFO("broadcasted");

    t1.Join();
    t2.Join();
    t3.Join();

    MWL_INFO("TestCondition done\n");
}
