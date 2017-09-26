#include "inc/Barrier.h"
#include "BarrierImplement.h"

namespace mwl {

    Barrier::Barrier(int32_t threadCount)
    : m_pImpl(new Implement(threadCount)) {}

    Barrier::~Barrier() {
        delete m_pImpl;
    }

    int32_t Barrier::Wait() {
            return 0;
    };
}
