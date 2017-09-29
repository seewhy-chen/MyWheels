#include "inc/Barrier.h"
#include "BarrierImplement.h"

namespace mwl {

    Barrier::Barrier(int32_t threshold)
    : m_pImpl(new Implement(threshold)) {}

    Barrier::~Barrier() {
        delete m_pImpl;
    }

    int32_t Barrier::Wait() {
        return m_pImpl->_Wait();
    };

    int32_t Barrier::SetThreshold(int32_t threshold) {
        return m_pImpl->_SetThreshold(threshold);
    }

    int32_t Barrier::Threshold() const {
        return m_pImpl->_Threshold();
    }
}
