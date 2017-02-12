#ifndef __MWL_THREAD_H__
#define __MWL_THREAD_H__

#include "inc/BasicDefines.h"
#include "inc/TimeDefines.h"
#include "inc/SmartPointer.h"

namespace mwl {

    struct MWL_API ThreadID {
        ThreadID();
        ThreadID(const ThreadID &rhs);
        ThreadID& operator=(const ThreadID &rhs);
        bool operator==(const ThreadID &rhs) const;

        unsigned long pid;
        unsigned long tid;
    };

    MWL_API void GetCurrentThreadID(ThreadID &threadID);

    class MWL_API ThreadContext : private NonCopyable {
    public:
        ThreadContext();
        ~ThreadContext();
        void QueryToStop();
        void *SharedData();
        bool StopQueried() const;
        const ThreadID& ParentID() const;
        const ThreadID& SelfID() const;
        const char *Tag() const;

        struct Implement;
        Implement *m_pImpl;
    };

    typedef int32_t (*SimpleThreadEntry)();
    typedef int32_t (*ThreadEntry)(ThreadContext *pContext);

    class MWL_API Thread : private NonCopyable {
    public:
        explicit Thread(const char *tag = nullptr);
        ~Thread();
        int32_t SetTag(const char *tag);
        int32_t Start(SimpleThreadEntry simpleEntry);
        int32_t Start(SimpleThreadEntry simpleEntry, const TimeSpec &timeout);
        int32_t Start(ThreadEntry entry);
        int32_t Start(ThreadEntry entry, void *pSharedData);
        int32_t Start(ThreadEntry entry, void *pSharedData, const TimeSpec &timeout);
        int32_t Stop(const TimeSpec &timeout = -1);
        int32_t Join(const TimeSpec &timeout = -1);

        void QueryToStop();
        void *SharedData();
        const ThreadID& ParentID() const;
        const ThreadID& SelfID() const;
        const char *Tag() const;
        bool IsRunning() const;
        int32_t ExitCode() const;

        struct Implement; 

    private:
        Implement *m_pImpl;
    };

    MWL_API SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry);
    MWL_API SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry, const char *tag);
    MWL_API SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry, const char *tag, const TimeSpec &timeout);

    MWL_API SharedPtr<Thread> StartThread(ThreadEntry entry);
    MWL_API SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData);
    MWL_API SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData, const char *tag);
    MWL_API SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData, const char *tag, const TimeSpec &timeout);

}

#endif
