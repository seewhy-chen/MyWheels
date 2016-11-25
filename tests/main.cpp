#include "common/BasicTypes.h"
#include "common/StaticAssert.h"

#include <cstdio>
#include <cstdlib>

using namespace mwl;

int main(int argc, char *argv[]) {
    MWL_STATIC_ASSERT(sizeof(int64_t) == 8, sizeof_int64_t_mistached);
    printf("sizeof(int64) = %d", sizeof(int64_t));    
    return 0;
}
