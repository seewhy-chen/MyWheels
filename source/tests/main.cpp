#include "inc/BasicDefines.h"

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
extern void TestFile();

const char *prog = nullptr;

int main(int , char *argv[]) {
    prog = argv[0];

    //TestBasicTypes();
    //TestByteArray();
    TestArray();
    //TestString();
    //TestDateTime();
    //TestTimeCounter();
    //TestThread();
    //TestMutex();
    //TestCondition();
    //TestSemaphore();
    //TestSocket();
    //TestFile();

    fprintf(stdout, "All tests passed\n");
#ifdef __MWL_WIN__
    fprintf(stdout, "Press ENTER to continue...\n");
    int32_t r = fgetc(stdin);
#endif

    return 0;
}
