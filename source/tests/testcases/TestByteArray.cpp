#include "inc/BasicDefines.h"
#include "inc/ByteArray.h"
using namespace mwl;

void TestByteArray() {
    MWL_INFO("TestByteArray started");
    uint8_t rawArr[] = {1,2,3,4,5,6,7,8,9};
    fprintf(stdout, "\n");
    MWL_INFO("rawArr:");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\n");

    ByteArray arr1(rawArr, sizeof(rawArr), OWN_SHARE);
    fprintf(stdout, "\n");
    MWL_INFO("arr1 shares rawArr:");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\n");

    ByteArray arr2;
    arr2.Share(arr1.Data(), arr1.Size());
    fprintf(stdout, "\n");
    MWL_INFO("arr2 shares arr1:");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\n");

    arr2[2] = 10;
    fprintf(stdout, "\n");
    MWL_INFO("after setting arr2[2] = 10:");
    MWL_INFO("rawArr:");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\n");
    MWL_INFO("arr1:");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1.At(i));
    }
    fprintf(stdout, "\n");
    MWL_INFO("arr2:");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\n");

    arr2.Fill(100);
    fprintf(stdout, "\n");
    MWL_INFO("after filling arr2 with 100...");
    MWL_INFO("rawArr:");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\n");
    MWL_INFO("arr1:");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\n");
    MWL_INFO("arr1:");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\n");

    ByteArray arr3;
    arr3 = arr2;
    fprintf(stdout, "\n");
    MWL_INFO("arr3 = arr2:");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\n");

    arr3[4] = 20;
    fprintf(stdout, "\n");
    MWL_INFO("after setting arr3[4] = 20:");
    MWL_INFO("rawArr:");
    for (int32_t i = 0; i < MWL_ARR_SIZE(rawArr); ++i) {
        fprintf(stdout, "%d, ", rawArr[i]);
    }
    fprintf(stdout, "\n");
    MWL_INFO("arr1:");
    for (int32_t i = 0; i < arr1.Size(); ++i) {
        fprintf(stdout, "%d, ", arr1[i]);
    }
    fprintf(stdout, "\n");
    MWL_INFO("arr1:");
    for (int32_t i = 0; i < arr2.Size(); ++i) {
        fprintf(stdout, "%d, ", arr2[i]);
    }
    fprintf(stdout, "\n");
    MWL_INFO("arr3:");
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "\n");
    MWL_INFO("after assign rawArr to arr3:");
    arr3.Assign(rawArr, MWL_ARR_SIZE(rawArr));
    for (int32_t i = 0; i < arr3.Size(); ++i) {
        fprintf(stdout, "%d, ", arr3[i]);
    }
    fprintf(stdout, "\n");

    arr3.Takeover(new uint8_t[100], 100);
    MWL_INFO("after taking over a ptr, arr3.Size() = %" JD, arr3.Size());
    MWL_INFO("%" JD" bytes copied to arr2 from arr3", arr2.Copy(arr3.Data(), arr3.Size()));

    fprintf(stdout, "\n");
    MWL_INFO("TestByteArray done\n");
}
