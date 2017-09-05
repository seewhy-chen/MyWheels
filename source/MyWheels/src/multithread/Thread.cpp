#include "inc/Thread.h"
#include "ThreadImplement.h"

namespace mwl {

    ThreadID::ThreadID() {
        pid = tid = -1;
    }

    ThreadID::ThreadID(const ThreadID &rhs) {
        pid = rhs.pid;
        tid = rhs.tid;
    }

    ThreadID &ThreadID::operator=(const ThreadID &rhs) {
        if (this != &rhs) {
            pid = rhs.pid;
            tid = rhs.tid;
        }
        return *this;
    }

    bool ThreadID::operator==(const ThreadID &rhs) const {
        return this == &rhs || (pid == rhs.pid && tid == rhs.tid);
    }

    Thread::Thread(const String &tag) : m_pImpl(new Implement()) {
        SetTag(tag);
    }

    Thread::~Thread() {
        delete m_pImpl;
    }

    int32_t Thread::SetTag(const String &tag) {
        return m_pImpl->_SetTag(tag);
    }

    static int32_t _SimpleThreadWrapper(ThreadContext *pControllee) {
        SimpleThreadEntry *entry = reinterpret_cast<SimpleThreadEntry *>(pControllee->SharedData());
        return (*entry)();
    }

    int32_t Thread::Start(SimpleThreadEntry simpleEntry) {
        return m_pImpl->_Start(_SimpleThreadWrapper, &simpleEntry, nullptr);
    }

    int32_t Thread::Start(SimpleThreadEntry simpleEntry, const TimeSpan &timeout) {
        return m_pImpl->_Start(_SimpleThreadWrapper, &simpleEntry, &timeout);
    }

    int32_t Thread::Start(ThreadEntry entry, void *pSharedData) {
        return m_pImpl->_Start(entry, pSharedData, nullptr);
    }

    int32_t Thread::Start(ThreadEntry entry, void *pSharedData, const TimeSpan &timeout) {
        return m_pImpl->_Start(entry, pSharedData, &timeout);
    }

    int32_t Thread::Stop() {
        return m_pImpl->_Stop(nullptr);
    }

    int32_t Thread::Stop(const TimeSpan &timeout) {
        return m_pImpl->_Stop(&timeout);
    }

    int32_t Thread::Join() {
        return m_pImpl->_Join(nullptr);
    }

    int32_t Thread::Join(const TimeSpan &timeout) {
        return m_pImpl->_Join(&timeout);
    }

    void Thread::QueryToStop() {
        return m_pImpl->_QueryToStop();
    }

    void *Thread::SharedData() {
        return m_pImpl->_SharedData();
    }

    const ThreadID &Thread::ParentID() const {
        return m_pImpl->_ParentID();
    }

    const ThreadID &Thread::SelfID() const {
        return m_pImpl->_SelfID();
    }

    const String& Thread::Tag() const {
        return m_pImpl->_Tag();
    }

    bool Thread::IsRunning() const {
        return m_pImpl->_IsRunning();
    }

    int32_t Thread::ExitCode() const {
        return m_pImpl->_ExitCode();
    }

    SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry, const String &tag, const TimeSpan &timeout) {
        SharedPtr<Thread> pThread(new Thread(tag));
        int32_t ret = pThread->Start(simpleEntry, timeout);
        if (ret != ERR_NONE) {
            pThread.reset();
        }
        return pThread;
    }

    SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData, const String &tag, const TimeSpan &timeout) {
        SharedPtr<Thread> pThread(new Thread(tag));
        int32_t ret = pThread->Start(entry, pSharedData, timeout);
        if (ret != ERR_NONE) {
            pThread.reset();
        }
        return pThread;
    }
}
