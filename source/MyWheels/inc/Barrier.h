#ifndef __MWL_BARRIER_H__
#define __MWL_BARRIER_H__

#include "inc/BasicDefines.h"

namespace mwl {
    class MWL_API Barrier : private NonCopyable {
    public:
        explicit Barrier(int32_t threshold = 1);
        ~Barrier();
        // return 1 for the last thread that reached the barrier, 0 for others, < 0 for failure
        int32_t Wait();
        int32_t SetThreshold(int32_t threshold);
        int32_t Threshold() const;
    private:
        struct Implement; 
        Implement *m_pImpl;
    };
}

#endif // __MWL_BARRIER_H__
