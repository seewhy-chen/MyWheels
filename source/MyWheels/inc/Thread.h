#ifndef __MWL_THREAD_H__
#define __MWL_THREAD_H__

#include "inc/BasicDefines.h"
#include "inc/String.h"
#include "inc/TimeSpan.h"
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
        const String &Tag() const;

        struct Implement;
        Implement *m_pImpl;
    };

    typedef int32_t(*SimpleThreadEntry)();
    typedef int32_t(*ThreadEntry)(ThreadContext *pContext);

    class MWL_API Thread : private NonCopyable {
    public:
        explicit Thread(const String &tag = String::Null());
        ~Thread();
        int32_t SetTag(const String &tag);
        int32_t Start(SimpleThreadEntry simpleEntry, const TimeSpan *pTimeout = nullptr);
        int32_t Start(ThreadEntry entry);
        int32_t Start(ThreadEntry entry, void *pSharedData, const TimeSpan *pTimeout = nullptr);
        int32_t Stop(const TimeSpan *pTimeout = nullptr);
        int32_t Join(const TimeSpan *pTimeout = nullptr);

        void QueryToStop();
        void *SharedData();
        const ThreadID& ParentID() const;
        const ThreadID& SelfID() const;
        const String &Tag() const;
        bool IsRunning() const;
        int32_t ExitCode() const;

        struct Implement;

    private:
        Implement *m_pImpl;
    };

    MWL_API SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry);
    MWL_API SharedPtr<Thread> StartThread(SimpleThreadEntry simpleEntry, const String &tag, const TimeSpan *pTimeout = nullptr);

    MWL_API SharedPtr<Thread> StartThread(ThreadEntry entry);
    MWL_API SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData);
    MWL_API SharedPtr<Thread> StartThread(ThreadEntry entry, void *pSharedData, const String &tag, const TimeSpan *pTimeout = nullptr);

}

#endif
