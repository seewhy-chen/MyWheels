#include "Thread.h"
#include "ThreadImplement.h"

namespace mwl {

    ThreadID::ThreadID() : m_pImpl(new Implement()) {}

    ThreadID::ThreadID(const ThreadID &rhs) : m_pImpl(new Implement()) {
        *m_pImpl = *rhs.m_pImpl;
    }

    ThreadID& ThreadID::operator=(const ThreadID &rhs) {
        if (this != &rhs) {
            *m_pImpl = *rhs.m_pImpl;
        }
        return *this;
    }

    ThreadID::~ThreadID() {
        delete m_pImpl;
    }

    uint32_t ThreadID::PID() const {
        return m_pImpl->pid;
    }

    uint32_t ThreadID::TID() const {
        return m_pImpl->tid;
    }

    bool ThreadID::operator==(const ThreadID &rhs) const {
        return this == &rhs || (m_pImpl->pid == rhs.m_pImpl->pid && m_pImpl->tid == rhs.m_pImpl->tid);
    }

    Thread::Thread() : m_pImpl(new Implement()) {
        m_pImpl->pThread = this;
    }

    Thread::~Thread() {
        delete m_pImpl;
    }

    void Thread::SetTag(const char *tag) {
        if (tag) {
            m_pImpl->tag = tag;
        } else {
            m_pImpl->tag.clear();
        }
    }

    int32_t Thread::Start(int32_t timeoutInMs) {
        return m_pImpl->_Start(timeoutInMs);
    }

    int32_t Thread::Stop(int32_t timeoutInMs) {
        return m_pImpl->_Stop(timeoutInMs);
    }

    int32_t Thread::Join(int32_t timeoutInMs) {
        return m_pImpl->_Join(timeoutInMs);
    }

    void Thread::QueryToStop() {
        return m_pImpl->_QueryToStop();
    }

    const ThreadID& Thread::Parent() const {
        return m_pImpl->_Parent();
    }

    const ThreadID& Thread::Self() const {
        return m_pImpl->_Self();
    }

    const char *Thread::Tag() const {
        return m_pImpl->_Tag();
    }

    bool Thread::IsRunning() const {
        return m_pImpl->_IsRunning();
    }

    int32_t Thread::ExitCode() const {
        return m_pImpl->_ExitCode();
    }

    bool Thread::StopQueried() {
        return m_pImpl->_StopQueried();
    }
}
