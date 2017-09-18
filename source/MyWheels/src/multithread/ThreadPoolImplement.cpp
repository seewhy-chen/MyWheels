#include "ThreadPoolImplement.h"
#include "ThreadContextImplement.h"

namespace mwl {
    struct ThreadPoolTaskRunnerData {
        ThreadPoolTaskRunnerData() {
            task = nullptr;
            pTaskData = nullptr;
            pIdleWorkers = nullptr;
            pCond = nullptr;
            pLock = nullptr;
            pBusyWorkerCnt = nullptr;
            pWorkDone = nullptr;
            pWorker = new Thread();
        }
        ~ThreadPoolTaskRunnerData() {
            delete pWorker;
        }

        ThreadEntry task;
        void *pTaskData;
        std::deque<ThreadPoolTaskRunnerData*> *pIdleWorkers;
        Condition *pCond;
        Mutex *pLock;
        int32_t *pBusyWorkerCnt;
        int32_t* pWorkDone;
        Thread *pWorker;
        Condition taskCond;
        Mutex taskMutex;
    };

    MWL_THREAD_ENTRY(_ThreadPoolTaskRunner, pContext) {
        ThreadPoolTaskRunnerData *pData = pContext->SharedData<ThreadPoolTaskRunnerData>();
        Mutex::AutoLock l(pData->taskMutex);
        while (!pContext->StopQueried()) {
            while (!pData->task && !pContext->StopQueried()) {
                pData->taskCond.Wait(pData->taskMutex);
            }
            if (!pContext->StopQueried() && pData->task) {
                pContext->m_pImpl->pSharedData = pData->pTaskData;
                pData->task(pContext);
                pData->pLock->Lock();
                pData->pIdleWorkers->push_back(pData);
                *pData->pBusyWorkerCnt -= 1;
                *pData->pWorkDone += 1;
                pData->task = nullptr;
                pData->pTaskData = nullptr;
                pData->pLock->Unlock();
                pData->pCond->Broadcast();
            }
        }
        return 0;
    }

    MWL_THREAD_ENTRY(_ThreadPoolEngine, pContext) {
        ThreadPool::Implement *pImpl = pContext->SharedData<ThreadPool::Implement>();
        Condition *pCond = &pImpl->_cond;
        std::deque<ThreadPoolTaskRunnerData*> idleWorkers;
        int32_t maxBusyWorkerCnt = 0;
        int32_t workDone = 0;
        while (!pContext->StopQueried()) {
            pImpl->_lock.Lock();
            while (pImpl->_HasNothingToDo() && !pContext->StopQueried()) {
                pImpl->_cond.Wait(pImpl->_lock);
                if (!pContext->StopQueried() && pImpl->_taskQ.empty() && !pImpl->_activeWorkerCnt && pImpl->_pListener) {
                    pImpl->_pListener->OnThreadPoolEvent(TPEVT_ALL_DONE, nullptr);
                }
            }
            if (pContext->StopQueried()) {
                break;
            }

            if (pImpl->_activeWorkerCnt + static_cast<int32_t>(idleWorkers.size()) < pImpl->_maxThreadCnt) {
                size_t newWorkersCnt = pImpl->_maxThreadCnt - idleWorkers.size() - pImpl->_activeWorkerCnt;
                if (newWorkersCnt > pImpl->_taskQ.size()) {
                    newWorkersCnt = pImpl->_taskQ.size();
                }
                for (size_t i = 0; i < newWorkersCnt; ++i) {
                    ThreadPoolTaskRunnerData *pTaskRunnerData = new ThreadPoolTaskRunnerData();
                    pTaskRunnerData->pIdleWorkers = &idleWorkers;
                    pTaskRunnerData->pCond = pCond;
                    pTaskRunnerData->pLock = &pImpl->_lock;
                    pTaskRunnerData->pBusyWorkerCnt = &pImpl->_activeWorkerCnt;
                    pTaskRunnerData->pWorkDone = &workDone;
                    if (pTaskRunnerData->pWorker->Start(_ThreadPoolTaskRunner, pTaskRunnerData) < 0) {
                        delete pTaskRunnerData;
                    } else {
                        idleWorkers.push_back(pTaskRunnerData);
                    }
                }
            }
            ThreadPoolTaskInfo taskInfo = pImpl->_taskQ.front();
            pImpl->_taskQ.pop_front();
            ThreadPoolTaskRunnerData *pTaskRunnerData = idleWorkers.front();
            idleWorkers.pop_front();
            pTaskRunnerData->taskMutex.Lock();
            pTaskRunnerData->task = taskInfo.task;
            pTaskRunnerData->pTaskData = taskInfo.pTaskData;
            pTaskRunnerData->taskMutex.Unlock();
            pTaskRunnerData->taskCond.Signal();
            ++pImpl->_activeWorkerCnt;
            if (pImpl->_activeWorkerCnt > maxBusyWorkerCnt) {
                maxBusyWorkerCnt = pImpl->_activeWorkerCnt;
            }
            pImpl->_lock.Unlock();
        }

        while (pImpl->_activeWorkerCnt > 0) {
            pCond->Wait(pImpl->_lock);
        }
        for (std::deque<ThreadPoolTaskRunnerData*>::iterator it = idleWorkers.begin(); it != idleWorkers.end(); ++it) {
            (*it)->taskMutex.Lock();
            (*it)->pWorker->QueryToStop();
            (*it)->taskMutex.Unlock();
            (*it)->taskCond.Signal();
            (*it)->pWorker->Join();
            delete *it;
        }
        pImpl->_lock.Unlock();
        MWL_INFO("maxThreadCnt = %d, actualThreadCnt = %d, maxBusyWorkerCnt = %d, workDone = %d", 
            pImpl->_maxThreadCnt, static_cast<int32_t>(idleWorkers.size()), maxBusyWorkerCnt, workDone);
        return 0;
    }

    ThreadPool::Implement::Implement(int32_t maxThreadCnt) 
    : _maxThreadCnt(0), _activeWorkerCnt(0), _taskID(0), _lock(true), _pListener(nullptr) {
        if (maxThreadCnt > 0) {
            _maxThreadCnt = maxThreadCnt;
        }
        _engine.Start(_ThreadPoolEngine, this);
    }

    ThreadPool::Implement::~Implement() {
        if (_lock.TryLock() < 0) {
            MWL_WARN("getting lock failed, current owenr is (%lu-%lu)", _lock.Owner().pid, _lock.Owner().tid);
            _lock.Lock();
        }
        _pListener = nullptr;
        _lock.Unlock();
        _Cancel();
    }

    int32_t ThreadPool::Implement::_SetMaxThreadCount(int32_t maxThreadCount) {
        Mutex::AutoLock l(_lock);
        bool moreWorkerAllowed = maxThreadCount > _maxThreadCnt;
        _maxThreadCnt = maxThreadCount > 0 ? maxThreadCount : 0;
        if (moreWorkerAllowed && !_taskQ.empty()) {
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

    int32_t ThreadPool::Implement::_BeginTaskPushing() {
        return _lock.Lock();
    }

    int32_t ThreadPool::Implement::_PushTask(ThreadEntry workEntry, void *pWorkData) {
        int32_t taskID = _taskID++;
        _taskQ.push_back(ThreadPoolTaskInfo(taskID, workEntry, pWorkData));
        return taskID;
    }

    int32_t ThreadPool::Implement::_CommitTaskPushing() {
        _lock.Unlock();
        return _cond.Broadcast();
    }

    int32_t ThreadPool::Implement::_Cancel() {
        _lock.Lock();
        _engine.QueryToStop();
        _lock.Unlock();
        _cond.Broadcast();
        return _engine.Join();
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
