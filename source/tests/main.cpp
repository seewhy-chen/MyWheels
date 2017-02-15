#include "inc/BasicDefines.h"
#include <stdlib.h>

#include "inc/Socket.h"

extern void TestBasicTypes();
extern void TestTimeCounter();
extern void TestThread();
extern void TestMutex();
extern void TestCondition();
extern void TestSemaphore();
extern void TestByteArray();
extern void TestSocket();

int main(int, char *[]) {
    //TestBasicTypes();
    //TestTimeCounter();
    //TestThread();
    //TestMutex();
    //TestCondition();
    //TestSemaphore();
    //TestByteArray();
    TestSocket();

#ifdef __MWL_WIN__
    system("pause");
#endif
    return 0;
}
