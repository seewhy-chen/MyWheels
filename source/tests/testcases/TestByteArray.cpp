#include "inc/BasicDefines.h"
#include "inc/ByteArray.h"
using namespace mwl;

void TestByteArray() {
    MWL_INFO("TestByteArray started");
    uint8_t rawArr[] = {1,2,3,4,5,6,7,8,9};
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\n");

    ByteArray arr1(rawArr, sizeof(rawArr), OWN_SHARE);
    fprintf(stdout, "\nafter arr1 sharing rawArr:");
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\n");

    ByteArray arr2;
    arr2.Share(arr1.Data(), arr1.Size());
    ByteArray::ConstIterator it = arr2.CBegin();
    ++it;
    *it = 13;
    fprintf(stdout, "\nafter arr2 sharing arr1:");
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\n");

    arr2[2] = 10;
    fprintf(stdout, "\nafter setting arr2[2] = 10:");
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1.At(i));
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\n");

    arr2.Copy(*arr2.Slice(1,4));
    fprintf(stdout, "\nafter copy arr2[1:3] to itself:");
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\n");


    ByteArray arr3;
    arr3 = arr2;
    fprintf(stdout, "\nafter arr3 = arr2:");
    fprintf(stdout, "\narr3:\t");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\n");

    SharedPtr<ByteArray> arr4 = arr3.Slice(3);
    fprintf(stdout, "\nafter arr4 = arr3.Slice(3):");
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    arr3[4] = 20;
    fprintf(stdout, "\nafter setting arr3[4] = 20:");
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\narr3:\t");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    arr3.Assign(rawArr, MWL_ARR_SIZE(rawArr));
    fprintf(stdout, "\nafter assign rawArr to arr3:");
    fprintf(stdout, "\narr3:\t");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    arr3.Assign(*arr3.Slice(3));
    fprintf(stdout, "\nafter assign arr3[3:] to itself:");
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\narr3:\t");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    arr2.Fill(100);
    fprintf(stdout, "\nafter filling arr2 with 100:");
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\narr3:\t");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    uint8_t*p = new uint8_t[100];
    memset(p, 0, 100);
    arr3.Takeover(p, 100);
    fprintf(stdout, "\nafter taking over a ptr, arr3.Size() = %d", arr3.Size());
    fprintf(stdout, "\nafter %d bytes copied from arr3 to arr2", arr2.Copy(arr3.Data(), arr3.Size()));
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    arr3.Takeover(arr2);
    fprintf(stdout, "\nafter arr3 taking over arr2:");
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\narr3:\t");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    arr3.Clear();
    fprintf(stdout, "\nafter arr3.Clear():");
    fprintf(stdout, "\nrawArr:\t");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\narr1:\t");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\narr2:\t");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\narr3:\t");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\narr4:\t");
    for (int32_t i = 0; i < arr4->Size(); ++i) {
        fprintf(stdout, "%d, ", arr4->At(i));
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "\n");
    MWL_INFO("TestByteArray done\n");
}
