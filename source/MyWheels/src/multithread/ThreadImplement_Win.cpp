#include "BasicDefines.h"

#ifdef __MWL_WIN__

#include "ThreadImplement.h"

namespace mwl {

    static DWORD _ThreadBody(void *data) {
        Thread::Implement *pImpl = reinterpret_cast<Thread::Implement *>(data);
        if (!pImpl) {
            MWL_WARN("pImpl is NULL");
        } else if (!pImpl->pThread) {
            MWL_WARN("pImpl->pThread is NULL");
        } else {
            pImpl->lock.Lock();
            pImpl->selfID.m_pImpl->pid = GetCurrentProcessId();
            pImpl->selfID.m_pImpl->tid = GetCurrentThreadId();
            pImpl->isRunning = true;
            pImpl->lock.Unlock();
            pImpl->cond.Signal();

            pImpl->exitCode = pImpl->pThread->Entry();

            pImpl->lock.Lock();
            pImpl->isRunning = false;
            pImpl->lock.Unlock();
            pImpl->cond.Signal();
        }
        return 0;
    }

    Thread::Implement::Implement() {
        threadHdl = 0;
        pThread = NULL;
        exitCode = -1;
        stopQueried = false;
        isRunning = false;
    }

    Thread::Implement::~Implement() {
        _Stop(-1);
    }

    int32_t Thread::Implement::_Start(int32_t timeoutInMs) {
        if (_IsRunning()) {
            return 0;
        }
         
        Mutex::AutoLock _l(lock);
        stopQueried = false;
        parentID.m_pImpl->pid = GetCurrentProcessId();
        parentID.m_pImpl->tid = GetCurrentThreadId();
        threadHdl = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(_ThreadBody), this, 0, &selfID.m_pImpl->tid);
        if (!threadHdl) {
            int32_t err = GetLastError();
            MWL_WARN_ERRNO("start thread failed", err);
            return -err;
        }

        int32_t ret = 0;
        do {
            ret = cond.Wait(lock, timeoutInMs);
        } while(!isRunning && ERR_TIMEOUT == ret);

        return isRunning ? 0 : ret;
    }

    int32_t Thread::Implement::_Stop(int32_t timeoutInMs) {
        if (!_IsRunning()) {
            return 0;
        }
        _QueryToStop();
        return _Join(timeoutInMs);
    }

    int32_t Thread::Implement::_Join(int32_t timeoutInMs) {
        int32_t ret = ERR_TIMEOUT;
        Mutex::AutoLock _l(lock);
        while(isRunning && ERR_TIMEOUT == ret) {
            ret = cond.Wait(lock, timeoutInMs);
        }
        if (!isRunning) {
            CloseHandle(threadHdl);
            threadHdl = 0;
        }

        return isRunning ? ERR_TIMEOUT : 0;
    }

    void Thread::Implement::_QueryToStop() {
        Mutex::AutoLock _l(lock);
        stopQueried = true;
    }

    bool Thread::Implement::_IsRunning() {
        Mutex::AutoLock _l(lock);
        return isRunning;
    }

    int32_t Thread::Implement::_ExitCode() {
        if (_IsRunning()) {
            return -1;
        }
        return exitCode;
    }

    bool Thread::Implement::_StopQueried() {
        Mutex::AutoLock _l(lock);
        return stopQueried;
    }

}

#endif // __MWL_WIN__
