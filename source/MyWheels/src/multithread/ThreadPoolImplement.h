#ifndef __MWL_THREAD_POOL_IMPLEMENT_H__
#define __MWL_THREAD_POOL_IMPLEMENT_H__

#include "inc/ThreadPool.h"
#include "inc/Array.h"
#include "inc/Mutex.h"
#include "inc/Semaphore.h"
#include "inc/Condition.h"

#include <deque>

namespace mwl {
    struct ThreadPoolTaskInfo {
        ThreadPoolTaskInfo(int32_t taskID_, ThreadEntry task_, void *pTaskData_) {
            taskID = taskID_;
            task = task_;
            pTaskData = pTaskData_;
        }
        int32_t taskID;
        ThreadEntry task;
        void* pTaskData;
    };

    struct ThreadPool::Implement {
        Implement(int32_t maxThreadCnt);
        ~Implement();
        int32_t _SetMaxThreadCount(int32_t maxThreadCount);
        int32_t _GetMaxThreadCount();
        int32_t _AddTask(ThreadEntry workEntry, void *pWorkData);
        int32_t _Join();
        int32_t _RegisterListener(ThreadPoolListener *pListener);
        inline bool _HasWorkToDo() {
            return !_taskQ.empty() || _activeWorkerCnt > 0;
        }

        inline bool _HasNothingToDo() {
            return _taskQ.empty() || _activeWorkerCnt >= _maxThreadCnt;
        }

        int32_t _maxThreadCnt;
        int32_t _activeWorkerCnt;
        int32_t _taskID;
        std::deque<ThreadPoolTaskInfo> _taskQ;
        Mutex _lock;
        Condition _cond;
        ThreadPoolListener *_pListener;
        Thread _engine;
    };
}

#endif // __MWL_THREAD_POOL_IMPLEMENT_H__