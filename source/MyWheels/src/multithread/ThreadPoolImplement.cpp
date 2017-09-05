#include "ThreadPoolImplement.h"
#include "ThreadContextImplement.h"

namespace mwl {
    struct ThreadPoolTaskRunnerData {
        ThreadEntry task;
        void *pTaskData;
        std::deque<Thread*> *pIdleWorkers;
        Condition *pCond;
        Mutex *pLock;
        int32_t *pBusyWorkerCnt;
        int32_t* pWorkDone;
        Thread *pWorker;
    };

    MWL_THREAD_ENTRY(_ThreadPoolTaskRunner, pContext) {
        ThreadPoolTaskRunnerData *pData = pContext->SharedData<ThreadPoolTaskRunnerData>();
        pContext->m_pImpl->pSharedData = pData->pTaskData;
        pData->task(pContext);
        pData->pLock->Lock();
        pData->pIdleWorkers->push_back(pData->pWorker);
        *pData->pBusyWorkerCnt -= 1;
        *pData->pWorkDone += 1;
        pData->pLock->Unlock();
        pData->pCond->Broadcast();
        delete pData;
        return 0;
    }

    MWL_THREAD_ENTRY(_ThreadPoolEngine, pContext) {
        ThreadPool::Implement *pImpl = pContext->SharedData<ThreadPool::Implement>();
        Condition *pCond = &pImpl->_cond;
        std::deque<Thread*> idleWorkers;
        int32_t maxBusyWorkerCnt = 0;
        int32_t workDone = 0;
        while (!pContext->StopQueried()) {
            pImpl->_lock.Lock();
            while (pImpl->_HasNothingToDo() && !pContext->StopQueried()) {
                pImpl->_cond.Wait(pImpl->_lock);
                if (!pContext->StopQueried() && pImpl->_taskQ.empty() && pImpl->_activeWorkerCnt == 0 && pImpl->_pListener) {
                    pImpl->_pListener->OnThreadPoolEvent(TPEVT_ALL_DONE, nullptr);
                }
            }
            if (pContext->StopQueried()) {
                break;
            }
            ThreadPoolTaskInfo taskInfo = pImpl->_taskQ.front();
            pImpl->_taskQ.pop_front();
            if (static_cast<int32_t>(idleWorkers.size()) < pImpl->_maxThreadCnt) {
                int32_t newWorkersCnt = pImpl->_maxThreadCnt - idleWorkers.size();
                for (int32_t i = 0; i < newWorkersCnt; ++i) {
                    idleWorkers.push_back(new Thread());
                }
            }
            Thread *pIdleWorker = idleWorkers.front();
            idleWorkers.pop_front();
            ThreadPoolTaskRunnerData *pTaskRunnerData = new ThreadPoolTaskRunnerData();
            pTaskRunnerData->task = taskInfo.task;
            pTaskRunnerData->pTaskData = taskInfo.pTaskData;
            pTaskRunnerData->pIdleWorkers = &idleWorkers;
            pTaskRunnerData->pCond = pCond;
            pTaskRunnerData->pLock = &pImpl->_lock;
            pTaskRunnerData->pBusyWorkerCnt = &pImpl->_activeWorkerCnt;
            pTaskRunnerData->pWorkDone = &workDone;
            pTaskRunnerData->pWorker = pIdleWorker;
            if (pIdleWorker->Start(_ThreadPoolTaskRunner, pTaskRunnerData) >= 0) {
                ++pImpl->_activeWorkerCnt;
                if (pImpl->_activeWorkerCnt > maxBusyWorkerCnt) {
                    maxBusyWorkerCnt = pImpl->_activeWorkerCnt;
                }
            }
            pImpl->_lock.Unlock();
        }

        while (pImpl->_activeWorkerCnt > 0) {
            pCond->Wait(pImpl->_lock);
        }
        for (std::deque<Thread*>::iterator it = idleWorkers.begin(); it != idleWorkers.end(); ++it) {
            delete *it;
        }
        pImpl->_lock.Unlock();
        MWL_INFO("maxBusyWorkerCnt = %d, workDone = %d", maxBusyWorkerCnt, workDone);
        return 0;
    }

    ThreadPool::Implement::Implement(int32_t maxThreadCnt) 
    : _maxThreadCnt(0), _activeWorkerCnt(0), _taskID(0), _pListener(nullptr) {
        if (maxThreadCnt > 0) {
            _maxThreadCnt = maxThreadCnt;
        }
        _engine.Start(_ThreadPoolEngine, this);
    }

    ThreadPool::Implement::~Implement() {
        _engine.QueryToStop();
        _cond.Broadcast();
        _engine.Join();
    }

    int32_t ThreadPool::Implement::_SetMaxThreadCount(int32_t maxThreadCount) {
        Mutex::AutoLock l(_lock);
        bool moreWorkerAllowed = maxThreadCount > _maxThreadCnt;
        _maxThreadCnt = maxThreadCount > 0 ? maxThreadCount : 0;
        if (moreWorkerAllowed) {
            _cond.Broadcast();
        }
        return _maxThreadCnt;
    }

    int32_t ThreadPool::Implement::_GetMaxThreadCount() {
        Mutex::AutoLock l(_lock);
        return _maxThreadCnt;
    }

    int32_t ThreadPool::Implement::_AddTask(ThreadEntry workEntry, void *pWorkData) {
        _lock.Lock();
        int32_t taskID = _taskID++;
        _taskQ.push_back(ThreadPoolTaskInfo(taskID, workEntry, pWorkData));
        _lock.Unlock();
        _cond.Broadcast();
        return taskID;
    }

    int32_t ThreadPool::Implement::_Join() {
        Mutex::AutoLock l(_lock);
        while (_HasWorkToDo()) {
            _cond.Wait(_lock);
        }
        return 0;
    }

    int32_t ThreadPool::Implement::_RegisterListener(ThreadPoolListener *pListener) {
        Mutex::AutoLock l(_lock);
        _pListener = pListener;
        return 0;
    }
}
