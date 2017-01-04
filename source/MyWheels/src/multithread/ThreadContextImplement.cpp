#include "inc/Thread.h"
#include "ThreadContextImplement.h"

namespace mwl {

    void ThreadContext::Implement::_QueryToStop() {
        Mutex::AutoLock _l(lock);
        stopQueried = true;
    }

    void* ThreadContext::Implement::_SharedData() {
        return pSharedData;
    }

    bool ThreadContext::Implement::_StopQueried() {
        Mutex::AutoLock _l(lock);
        return stopQueried;
    }

    const ThreadID& ThreadContext::Implement::_ParentID() {
        return parentID;
    }

    const ThreadID& ThreadContext::Implement::_SelfID() {
        return selfID;
    }

    const char* ThreadContext::Implement::_Tag() {
        return tag.c_str();
    }

}
