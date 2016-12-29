#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Condition.h"
#include "inc/Mutex.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct MyThread : public Thread {
    Condition &_cond;
    Mutex &_mutex;
    MyThread(const char *tag, Condition &cond, Mutex &mutex) : _cond(cond), _mutex(mutex) {
        SetTag(tag);
    }
    int32_t Entry() {
        MWL_INFO("(%u, %u) %s started, parent is (%u, %u)", 
            Self().PID(), Self().TID(), Tag(), Parent().PID(), Parent().TID());

        _mutex.Lock();
        _cond.Wait(_mutex, 10);
        _mutex.Unlock();
        MWL_INFO("(%u, %u) %s stopped", Self().PID(), Self().TID(), Tag());

        return 0;
    }
};

void TestThread() {
    Condition cond;
    Mutex mutex;
    MyThread t1("t1", cond, mutex), t2("t2", cond, mutex), t3("t3", cond, mutex);
    t1.Start();
    t2.Start();
    t3.Start();
    TimeSleep(1000);
    //cond.Signal();
    //cond.Signal();
    //cond.Signal();
    t1.Join();
    t2.Join();
    t3.Join();
}
