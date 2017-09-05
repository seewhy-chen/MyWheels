#include "inc/ThreadPool.h"
#include "ThreadPoolImplement.h"

namespace mwl {

    ThreadPool::ThreadPool(int32_t maxThreadCount)
    : m_pImpl(new Implement(maxThreadCount)){}

    ThreadPool::~ThreadPool() {
        delete m_pImpl;
    }

    int32_t ThreadPool::SetMaxThreadCount(int32_t maxThreadCount) {
        return m_pImpl->_SetMaxThreadCount(maxThreadCount);
    }

    int32_t ThreadPool::GetMaxThreadCount() const {
        return m_pImpl->_GetMaxThreadCount();
    }

    int32_t ThreadPool::AddTask(ThreadEntry workEntry, void *pWorkData) {
        return m_pImpl->_AddTask(workEntry, pWorkData);
    }

    int32_t ThreadPool::Cancel() {
        return m_pImpl->_Cancel();
    }

    int32_t ThreadPool::Join() {
        return m_pImpl->_Join();
    }

    int32_t ThreadPool::RegisterListener(ThreadPoolListener *pListener) {
        return m_pImpl->_RegisterListener(pListener);
    }
}
