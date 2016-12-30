#include "inc/BasicDefines.h"
#include "inc/TimeCounter.h"
using namespace mwl;

void TestTimeCounter() {
    MWL_INFO("TestThread started");

    TimeCounter counter;
    counter.Start();
    TimeSleep(1000);
    counter.Start("counter-with-tag");
    uint64_t t = counter.TimeElapsed(MILLISEC);
    MWL_INFO("%" JD"ms passed", t);

    TimeSleep(1000);
    t = counter.TimeElapsed("not-exist", MILLISEC);
    MWL_INFO("%" JD"ms passed for counter 'not-exist'", t);
    t = counter.TimeElapsed("counter-with-tag", MILLISEC);
    MWL_INFO("%" JD"ms passed for counter 'counter-with-tag'", t);

    MWL_INFO("TestThread done\n");
}
