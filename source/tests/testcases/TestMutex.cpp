#include "inc/Thread.h"
#include "inc/Mutex.h"
#include "inc/TimeDefines.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct MutexThread : public Thread {
    Mutex &mutex;
    MutexThread(const char *tag, Mutex &mutex_) : mutex(mutex_) {
        SetTag(tag);
    }

    int32_t Entry() {
        MWL_INFO("%s started", Tag());
        TimeSleep(500);
        mutex.Lock();
        MWL_INFO("%s got the mutex", Tag());
        TimeSleep(500);
        mutex.Unlock();
        MWL_INFO("%s stopped", Tag());
        return 0;
    }
};

void TestMutex() {
    MWL_INFO("TestMutex started");

    Mutex mutex;
    MutexThread t1("t1", mutex), t2("t2", mutex), t3("t3", mutex);
    t1.Start();
    t2.Start();
    t3.Start();

    t1.Join();
    t2.Join();
    t3.Join();

    MWL_INFO("TestMutex done\n");
}
