#include "BasicDefines.h"

#include <stdlib.h>

extern void TestBasicTypes();
extern void TestMutex();
extern void TestTimeCounter();
extern void TestThread();

int main(int argc, char *argv[]) {
    TestBasicTypes();
    TestMutex();
    TestTimeCounter();
    TestThread();

#ifdef __MWL_WIN__
	system("pause");
#endif
    return 0;
}
