#ifndef __MWL_MUTEX_H__
#define __MWL_MUTEX_H__

#include "BasicTypes.h"

namespace mwl {
    class Mutex {
        public:
            Mutex();
            ~Mutex();
            int32_t Lock();
            int32_t Unlock();

        private:
            struct Implement *m_pImpl;
    };

} /* namespace mwl */

#endif
