#ifndef __MWL_STRING_IMPLEMENT_H__
#define __MWL_STRING_IMPLEMENT_H__

#include "InternalCommon.h"
#include "inc/String.h"

namespace mwl {

    struct String::Implement {
        int32_t _capacity;
        int32_t _len;
        char *_pContent;
        static const int32_t _BASE_SIZE;
        void _Copy(const void* pSrc, int32_t len);
        void _Cat(const void* pSrc, int32_t len);
    };

}

#endif // __MWL_STRING_IMPLEMENT_H__
