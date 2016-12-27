#include "BasicDefines.h"
#include "TimeCounter.h"
using namespace mwl;

void TestTimeCounter() {
    TimeCounter counter;
    counter.Start();
    TimeSleep(1000);
    counter.Start("counter-with-tag");
    uint64_t t = counter.TimeElapsed(MICROSEC);
    MWL_INFO("%lld ms passed", t);

    TimeSleep(1000);
    t = counter.TimeElapsed("not-exist", MICROSEC);
    MWL_INFO("%lld ms passed for counter 'not-exist'", t);
    t = counter.TimeElapsed("counter-with-tag", MICROSEC);
    MWL_INFO("%lld ms passed for counter 'counter-with-tag'", t);
}
