#include "BasicDefines.h"
#include "TimeCounter.h"
using namespace mwl;

void TestTimeCounter() {
    TimeCounter counter;
    counter.Start();
    TimeSleep(1000);
    uint64_t t = counter.TimeElapsed(MICROSEC);
    MWL_LOG("%lld ms passed", t);
}
