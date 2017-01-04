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

    Thread::Thread(const char *tag) : m_pImpl(new Implement()) {
        SetTag(tag);
    }

    Thread::~Thread() {
        delete m_pImpl;
    }

    int32_t Thread::SetTag(const char *tag) {
        return m_pImpl->_SetTag(tag);
    }

    static int32_t _SimpleThreadWrapper(ThreadContext *pControllee) {
        SimpleThreadEntry *entry = reinterpret_cast<SimpleThreadEntry *>(pControllee->SharedData());
        return (*entry)();
    }

    int32_t Thread::Start(SimpleThreadEntry simpleEntry) {
        return Start(simpleEntry, -1);
    }

    int32_t Thread::Start(SimpleThreadEntry simpleEntry, int32_t timeoutInMs) {
        return m_pImpl->_Start(_SimpleThreadWrapper, &simpleEntry, timeoutInMs);
    }

    int32_t Thread::Start(ThreadEntry entry) {
        return m_pImpl->_Start(entry, NULL, -1);
    }

    int32_t Thread::Start(ThreadEntry entry, void *pSharedData) {
        return m_pImpl->_Start(entry, pSharedData, -1);
    }

    int32_t Thread::Start(ThreadEntry entry, void *pSharedData, int32_t timeoutInMs) {
        return m_pImpl->_Start(entry, pSharedData, timeoutInMs);
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

    void *Thread::SharedData() {
        return m_pImpl->_SharedData();
    }

    const ThreadID &Thread::ParentID() const {
        return m_pImpl->_ParentID();
    }

    const ThreadID &Thread::SelfID() const {
        return m_pImpl->_SelfID();
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

    SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry) {
        return StartThread(simpleEntry, NULL, -1);
    }

    SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry, const char *tag) {
        return StartThread(simpleEntry, tag, -1);
    }

    SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry, const char *tag, int32_t timeoutInMs) {
        SharedPtr<Thread> pThread(new Thread(tag));
        int32_t ret = pThread->Start(simpleEntry, timeoutInMs);
        if (ret != ERR_NONE) {
            pThread.reset();
        }
        return pThread;
    }

    SharedPtr<Thread> StartThread(ThreadEntry entry) {
        return StartThread(entry, NULL, NULL, -1);
    }

    SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData) {
        return StartThread(entry, pSharedData, NULL, -1);
    }

    SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData, const char *tag) {
        return StartThread(entry, pSharedData, tag, -1);
    }

    SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData, const char *tag, int32_t timeoutInMs) {
        SharedPtr<Thread> pThread(new Thread(tag));
        int32_t ret = pThread->Start(entry, pSharedData, timeoutInMs);
        if (ret != ERR_NONE) {
            pThread.reset();
        }
        return pThread;
    }
}
