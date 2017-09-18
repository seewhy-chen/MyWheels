#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__
#include <vld.h>
#endif

extern void TestBasicTypes();
extern void TestTimeCounter();
extern void TestMutex();
extern void TestCondition();
extern void TestSemaphore();
extern void TestByteArray();
extern void TestOptionParser();
extern void TestSocket();
extern void TestArray();
extern void TestString();
extern void TestDateTime();
extern void TestFile();
extern void TestThread();
extern void TestThreadPool();

const char *prog = nullptr;

int main(int , char *argv[]) {
    prog = argv[0];

    //TestBasicTypes();
    //TestByteArray();
    //TestArray();
    //TestString();
    //TestDateTime();
    //TestTimeCounter();
    //TestSocket();
    //TestFile();
    //TestMutex();
    //TestCondition();
    //TestSemaphore();
    //TestThread();
    //TestThreadPool();
    TestOptionParser();

    fprintf(stdout, "All tests passed\n");
#ifdef __MWL_WIN__
    fprintf(stdout, "Press ENTER to continue...\n");
    int32_t r = fgetc(stdin);
#endif

    return 0;
}
