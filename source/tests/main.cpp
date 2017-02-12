#include "inc/BasicDefines.h"

#include <stdlib.h>

extern void TestBasicTypes();
extern void TestTimeCounter();
extern void TestThread();
extern void TestMutex();
extern void TestCondition();
extern void TestSemaphore();
extern void TestByteArray();

int main(int, char *[]) {
    TestBasicTypes();
    TestTimeCounter();
    TestThread();
    TestMutex();
    TestCondition();
    TestSemaphore();
    TestByteArray();

#ifdef __MWL_WIN__
    system("pause");
#endif
    return 0;
}
