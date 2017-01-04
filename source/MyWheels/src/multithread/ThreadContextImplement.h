#ifndef __MWL_THREAD_CONTROLLEE_IMPLEMENT_H__
#define __MWL_THREAD_CONTROLLEE_IMPLEMENT_H__

#include "inc/Thread.h"
#include "inc/Mutex.h"
#include "inc/Condition.h"

#include <string>

namespace mwl {

    struct ThreadContext::Implement {
        ThreadID parentID;
        ThreadID selfID;
        std::string tag;

        Mutex lock;
        Condition cond;
        void *pSharedData;

        int32_t exitCode;
        bool stopQueried;
        bool isRunning;

        Implement();
        void _QueryToStop();
        void *_SharedData();
        bool _StopQueried();
        const ThreadID& _ParentID();
        const ThreadID& _SelfID();
        const char *_Tag();
    };

}

#endif // __MWL_THREAD_CONTROLLEE_IMPLEMENT_H__
