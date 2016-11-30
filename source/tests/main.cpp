#include "Mutex.h"
using namespace mwl;

extern void TestBasicTypes();
extern void TestMutex();


int main(int argc, char *argv[]) {
    TestBasicTypes();
    TestMutex();
    return 0;
}
