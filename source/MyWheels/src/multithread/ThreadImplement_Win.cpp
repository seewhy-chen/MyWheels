#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "inc/Thread.h"
#include "ThreadImplement.h"
#include "ThreadContextImplement.h"

namespace mwl {

    void GetCurrentThreadID(ThreadID &threadID) {
        threadID.pid = ::GetCurrentProcessId();
        threadID.tid = ::GetCurrentThreadId();
    }

    static DWORD _ThreadBody(void *data);

    int32_t Thread::Implement::_Start(ThreadEntry entry, void *pSharedData, const TimeSpan *pTimeout) {
        Mutex::AutoLock _l(context.m_pImpl->lock);
        if (context.m_pImpl->isRunning) {
            return ERR_NONE;
        }
        context.m_pImpl->stopQueried = false;
        mwl::GetCurrentThreadID(context.m_pImpl->parentID);
        context.m_pImpl->pSharedData = pSharedData;
        this->entry = entry;

        threadHdl = ::CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(_ThreadBody), this, 0, &context.m_pImpl->selfID.tid);
        if (!threadHdl) {
            int32_t err = GetLastError();
            MWL_WARN_ERRNO("start thread failed", err);
            return -err;
        }

        int32_t ret = 0;
        if (pTimeout) {
            do {
                ret = context.m_pImpl->cond.Wait(context.m_pImpl->lock, *pTimeout);
            } while (!context.m_pImpl->isRunning && ERR_TIMEOUT == ret);
        } else {
            do {
                ret = context.m_pImpl->cond.Wait(context.m_pImpl->lock);
            } while (!context.m_pImpl->isRunning && ERR_TIMEOUT == ret);
        }

        return context.m_pImpl->isRunning ? ERR_NONE : ret;
    }

    int32_t Thread::Implement::_Join(const TimeSpan *pTimeout) {
        int32_t ret = ERR_TIMEOUT;
        Mutex::AutoLock _l(context.m_pImpl->lock);
        if (pTimeout) {
            while (context.m_pImpl->isRunning && ERR_TIMEOUT == ret) {
                ret = context.m_pImpl->cond.Wait(context.m_pImpl->lock, *pTimeout);
            }
        } else {
            while (context.m_pImpl->isRunning && ERR_TIMEOUT == ret) {
                ret = context.m_pImpl->cond.Wait(context.m_pImpl->lock);
            }
        }
        if (!context.m_pImpl->isRunning) {
            CloseHandle(threadHdl);
            threadHdl = 0;
        }

        return context.m_pImpl->isRunning ? ERR_TIMEOUT : 0;
    }

    static DWORD _ThreadBody(void *data) {
        Thread::Implement *pThreadImpl = reinterpret_cast<Thread::Implement *>(data);
        if (!pThreadImpl) {
            MWL_WARN("pThreadImpl is NULL");
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
        return 0;
    }

}

#endif // __MWL_WIN__

