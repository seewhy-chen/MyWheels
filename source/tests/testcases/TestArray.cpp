#include "inc/Array.h"
#include "inc/String.h"
#include "InternalCommon.h"
using namespace mwl;

void TestArray() {
    Array<String> s(10, "1");
    MWL_ASSERT(s.Size() == 10);
    MWL_ASSERT(s.Front() == "1");
    MWL_ASSERT(s[0] == "1");
    MWL_ASSERT(s[-1] == "1");

    Array<String> s2(s);
    Array<String> s3;
    s3 = s;

    MWL_ASSERT(s == s2);
    MWL_ASSERT(s == s3);

    s.Resize(20, "2");
    MWL_ASSERT(s.Size() == 20);
    MWL_ASSERT(s[0] == "1");
    MWL_ASSERT(s[-1] == "2");

    s.Append("3");
    MWL_ASSERT(s.Back() == "3");
    MWL_ASSERT(s[-1] == "3");
    MWL_ASSERT(s.Size() == 21);

    s.Insert(0, "4");
    MWL_ASSERT(s[0] == "4");

    s.Insert(-1, "5");
    MWL_ASSERT(s[-2] == "5");
    MWL_ASSERT(s.Size() == 23);

    s.Remove(0);
    MWL_ASSERT(s.Size() == 22);
    s.Remove(-1);
    MWL_ASSERT(s.Size() == 21);
    s.Remove(0, s.Size());
    MWL_ASSERT(s.Size() == 0);
    MWL_ASSERT(s.Empty());


}
