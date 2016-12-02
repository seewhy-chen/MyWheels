#include "BasicDefines.h"

#include <stdlib.h>

extern void TestBasicTypes();
extern void TestMutex();
extern void TestTimeCounter();

int main(int argc, char *argv[]) {
    TestBasicTypes();
    TestMutex();
    TestTimeCounter();

#ifdef __WINDOWS__
	system("pause");
#endif
    return 0;
}
