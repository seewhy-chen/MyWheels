#include "inc/BasicDefines.h"
#include "inc/ByteArray.h"
using namespace mwl;

void TestByteArray() {
    MWL_INFO("TestByteArray started");
    ByteArray arr1(100);
    ByteArray arr2;
    arr2.Share(arr1);
    arr2.Resize(200);
    int64_t s = arr1.Size();
    ByteArray arr3;
    arr3 = arr2;
    arr3.Resize(10);
    s = arr3.Size();
    s = arr1.Size();

    arr3.Clear();
    s = arr3.Size();
    s = arr1.Size();

    arr2.Clear();
    s = arr1.Size();

    MWL_INFO("TestByteArray done\n");
}
