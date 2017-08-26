#include "inc/BasicDefines.h"
#include <stdlib.h>

extern void TestBasicTypes();
extern void TestTimeCounter();
extern void TestThread();
extern void TestMutex();
extern void TestCondition();
extern void TestSemaphore();
extern void TestByteArray();
extern void TestSocket();
extern void TestArray();
extern void TestString();
extern void TestDateTime();

#include <string>

int main(int, char *[]) {
    //TestBasicTypes();
    TestArray();
    TestString();
    TestDateTime();
    //TestTimeCounter();
    //TestThread();
    //TestMutex();
    //TestCondition();
    //TestSemaphore();
    //TestByteArray();
    //TestSocket();

    fprintf(stdout, "All tests passed\n");
#ifdef __MWL_WIN__
    system("pause");
#endif
    return 0;
}
