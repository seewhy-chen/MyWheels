#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Semaphore.h"

#include "InternalCommon.h"
using namespace mwl;

#include <string>

struct SemaphoreThread : public Thread {
    SemaphoreThread(const char *tag) {
        SetTag(tag);
    }
    int32_t Entry() {
        Semaphore sem;
        sem.Open("test", 0);
        MWL_INFO("%s started", Tag());
        if (sem.Wait(1000) == ERR_TIMEOUT) {
            MWL_INFO("%s wait semaphore timeout", Tag()); 
        } else {
            MWL_INFO("%s got semaphore, post it", Tag()); 
            sem.Post();
        }
        MWL_INFO("%s stopped", Tag());
        return 0;
    }
};

void TestSemaphore() {
    MWL_INFO("TestSemaphore started");

    Semaphore sem;
    sem.Open("test", 0);

    SemaphoreThread t1("t1"), t2("t2"), t3("t3");
    t1.Start();
    t2.Start();
    t3.Start();

    TimeSleep(500);
    MWL_INFO("posting...");
    sem.Post();

    t1.Join();
    t2.Join();
    t3.Join();

    MWL_INFO("TestSemaphore done\n");
}
