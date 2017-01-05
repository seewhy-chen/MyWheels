#include "inc/BasicDefines.h"
#include "inc/ByteArray.h"
using namespace mwl;

void TestByteArray() {
    MWL_INFO("TestByteArray started");
    ByteArray arr1(100);
    ByteArray arr2;
    arr2.ShareConst(arr1);

    MWL_INFO("TestByteArray done\n");
}
