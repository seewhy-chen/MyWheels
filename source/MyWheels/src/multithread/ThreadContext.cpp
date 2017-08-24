#include "inc/Thread.h"
#include "ThreadContextImplement.h"

namespace mwl {

    ThreadContext::ThreadContext() {
        m_pImpl = new Implement();
    }

    ThreadContext::~ThreadContext() {
        delete m_pImpl;
    }

    void ThreadContext::QueryToStop() {
        return m_pImpl->_QueryToStop();
    }

    void *ThreadContext::SharedData() {
        return m_pImpl->_SharedData();
    }

    bool ThreadContext::StopQueried() const {
        return m_pImpl->_StopQueried();
    }

    const ThreadID& ThreadContext::ParentID() const {
        return m_pImpl->_ParentID();
    }

    const ThreadID& ThreadContext::SelfID() const {
        return m_pImpl->_SelfID();
    }

    const String& ThreadContext::Tag() const {
        return m_pImpl->_Tag();
    }

}
