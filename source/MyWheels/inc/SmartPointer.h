#ifndef __MWL_SMART_POINTER_H__
#define __MWL_SMART_POINTER_H__

#include "BasicDefines.h"
#include <memory>

namespace mwl {

#if defined __CXX_11__
    #define SharedPtr std::shared_ptr
    #define WeakPtr std::weak_ptr
    #define UniquePtr std::unique_ptr
#else
    #define SharedPtr std::tr1::shared_ptr
    #define WeakPtr std::tr1::weak_ptr
    #define UniquePtr std::auto_ptr
#endif

}

#endif // __MWL_SMART_POINTER_H__
