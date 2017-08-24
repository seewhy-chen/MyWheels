#ifndef __MWL_TEST_INTERNAL_COMMON_H__
#define __MWL_TEST_INTERNAL_COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef WINVER
#define MWL_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stdout, "%s (%d):\n  assert(%s) FAILED\n\n", __FILE__, __LINE__, #condition); \
            system("pause"); \
            exit(-1); \
        } \
    } while(0)
#else 
#define MWL_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stdout, "%s (%d):\n  assert(%s) FAILED\n\n", __FILE__, __LINE__, #condition); \
            exit(-1); \
        } \
    } while(0)

#endif

#endif // __MWL_TEST_INTERNAL_COMMON_H__
