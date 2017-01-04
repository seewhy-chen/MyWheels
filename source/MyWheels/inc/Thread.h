#ifndef __MWL_THREAD_H__
#define __MWL_THREAD_H__

#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__
#include <pthread.h>
#endif

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
        explicit Thread(const char *tag = NULL);
        ~Thread();
        int32_t SetTag(const char *tag);
        int32_t Start(SimpleThreadEntry simpleEntry);
        int32_t Start(SimpleThreadEntry simpleEntry, int32_t timeoutInMs);
        int32_t Start(ThreadEntry entry);
        int32_t Start(ThreadEntry entry, void *pSharedData);
        int32_t Start(ThreadEntry entry, void *pSharedData, int32_t timeoutInMs);
        int32_t Stop(int32_t timeoutInMs = -1);
        int32_t Join(int32_t timeoutInMs = -1);

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

    // TODO: wrap following return value with shared_ptr
    Thread* StartThread(SimpleThreadEntry simpleEntry);
    Thread* StartThread(SimpleThreadEntry simpleEntry, const char *tag);
    Thread* StartThread(SimpleThreadEntry simpleEntry, const char *tag, int32_t timeoutInMs);

    Thread* StartThread(ThreadEntry entry);
    Thread* StartThread(ThreadEntry entry, void *pSharedData);
    Thread* StartThread(ThreadEntry entry, void *pSharedData, const char *tag);
    Thread* StartThread(ThreadEntry entry, void *pSharedData, const char *tag, int32_t timeoutInMs);

}

#endif
