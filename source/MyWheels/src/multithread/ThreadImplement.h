#ifndef __MWL_THREAD_IMPLEMENT_H__
#define __MWL_THREAD_IMPLEMENT_H__

#include "InternalCommon.h"

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

#include <string>

namespace mwl {
    struct ThreadID::Implement {
#ifdef __MWL_WIN__
        DWORD pid;
        DWORD tid;
#else
        pid_t pid;
        pthread_t tid;
#endif
    };

    struct Thread::Implement {
        Implement();
        ~Implement();
        int32_t _Start(int32_t timeoutInMs);
        int32_t _Stop(int32_t timeoutInMs);
        int32_t _Join(int32_t timeoutInMs);
        void _QueryToStop();
        bool _IsRunning();
        int32_t _ExitCode();

        void _ThreadStarted();
        bool _StopQueried();

        inline const ThreadID& _Parent() {
            return parentID;
        }

        inline const ThreadID& _Self() {
            return selfID;
        }

        inline const char *_Tag() {
            return tag.c_str();
        }

#ifdef __MWL_WIN__
        HANDLE threadHdl;
#else
#endif

        Thread *pThread;
        ThreadID parentID;
        ThreadID selfID;
        std::string tag;

        Mutex lock;
        Condition cond;

        int32_t exitCode;
        bool stopQueried;
        bool isRunning;
    };
}

#endif
