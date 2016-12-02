#include "BasicDefines.h"
#include "StaticAssert.h"
#include "TimeCounter.h"
using namespace mwl;

void TestBasicTypes() {
    MWL_STATIC_ASSERT(sizeof(int8_t) == 1, sizeof_int8_mismatched);
    MWL_STATIC_ASSERT(sizeof(uint8_t) == 1, sizeof_uint8_mismatched);
    MWL_STATIC_ASSERT(sizeof(int16_t) == 2, sizeof_int16_mismatched);
    MWL_STATIC_ASSERT(sizeof(uint16_t) == 2, sizeof_uint16_mismatched);
    MWL_STATIC_ASSERT(sizeof(int32_t) == 4, sizeof_int32_mismatched);
    MWL_STATIC_ASSERT(sizeof(uint32_t) == 4, sizeof_uint32_mismatched);
    MWL_STATIC_ASSERT(sizeof(int64_t) == 8, sizeof_int64_mismatched);
    MWL_STATIC_ASSERT(sizeof(uint64_t) == 8, sizeof_uint64_mismatched);
}
