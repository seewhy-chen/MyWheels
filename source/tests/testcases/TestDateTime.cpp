#include "inc/DateTime.h"

#include "InternalCommon.h"
using namespace mwl;

void TestDateTime() {
    String s;
    String fmt("%Y-%m-%d");
    String fmt2("%Y-%m-%d %H:%M:%S");
    DateTime now = DateTime::Now(DateTime::TZ_LOCAL);
    MWL_INFO("now(local):\t%s", now.ToStr().C_Str());
    MWL_INFO("now(utc):\t%s", now.AsUTC().ToStr().C_Str());

    TimeSpan offset(24, HOUR);
    DateTime dt1;
    dt1.FromStr("2017-8-1", fmt);
    DateTime dt2 = dt1 + offset;
    MWL_ASSERT(dt2.Year() == 2017);
    MWL_ASSERT(dt2.Month() == 7);
    MWL_ASSERT(dt2.MonthDay() == 2);
    MWL_ASSERT(dt2 - dt1 == offset);
    MWL_ASSERT(dt1 - dt2 == -offset);
}
