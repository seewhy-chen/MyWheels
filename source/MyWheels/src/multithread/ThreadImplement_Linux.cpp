#include "inc/BasicDefines.h"

#ifdef __MWL_LINUX__

#include "ThreadImplement.h"
#include "ThreadContextImplement.h"

#include <sys/syscall.h>
#define __mwl_gettid() syscall(SYS_gettid)

namespace mwl {

    void GetCurrentThreadID(ThreadID &threadID) {
        threadID.pid = getpid();
        threadID.tid = __mwl_gettid();
    }

    static void *_ThreadBody(void *data);

    int32_t Thread::Implement::_Start(ThreadEntry entry, void *pSharedData, const TimeSpan *pTimeout) {
        Mutex::AutoLock _l(context.m_pImpl->lock);
        if (context.m_pImpl->isRunning) {
            return ERR_NONE;
        }
        context.m_pImpl->stopQueried = false;
        mwl::GetCurrentThreadID(context.m_pImpl->parentID);
        context.m_pImpl->pSharedData = pSharedData;
        this->entry = entry;

        if (pthread_create(&context.m_pImpl->selfID.tid, nullptr, _ThreadBody, this) != 0) {
            int32_t err = errno;
            MWL_WARN_ERRNO("create thread failed", err);
            return -err;
        }

        int32_t ret = 0;
        do {
            ret = context.m_pImpl->cond.Wait(context.m_pImpl->lock, pTimeout);
        } while(!context.m_pImpl->isRunning && ERR_TIMEOUT == ret);

        return context.m_pImpl->isRunning ? ERR_NONE : ret;
    }

    int32_t Thread::Implement::_Join(const TimeSpan *pTimeout) {
        int32_t ret = ERR_TIMEOUT;
        Mutex::AutoLock _l(context.m_pImpl->lock);
        while (context.m_pImpl->isRunning && ERR_TIMEOUT == ret) {
            ret = context.m_pImpl->cond.Wait(context.m_pImpl->lock, pTimeout);
        }
        return context.m_pImpl->isRunning ? ERR_TIMEOUT : 0;
    }

    static void* _ThreadBody(void *data) {
        pthread_detach(pthread_self());
        Thread::Implement *pThreadImpl = reinterpret_cast<Thread::Implement *>(data);
        if (!pThreadImpl) {
            MWL_WARN("pThreadImpl is nullptr");
        } else {
            ThreadContext &context = pThreadImpl->context;
            context.m_pImpl->lock.Lock();
            mwl::GetCurrentThreadID(context.m_pImpl->selfID);
            context.m_pImpl->isRunning = true;
            context.m_pImpl->lock.Unlock();
            context.m_pImpl->cond.Signal();

            context.m_pImpl->exitCode = pThreadImpl->entry(&context);

            context.m_pImpl->lock.Lock();
            context.m_pImpl->isRunning = false;
            context.m_pImpl->lock.Unlock();
            context.m_pImpl->cond.Signal();
        }
        return nullptr;
    }
}

#endif // __MWL_LINUX__
