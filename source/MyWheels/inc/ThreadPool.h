#ifndef __MWL_THREAD_POOL_H__
#define __MWL_THREAD_POOL_H__

#include "inc/BasicDefines.h"
#include "inc/Thread.h"

namespace mwl {
    enum ThreadPoolEvent {
        TPEVT_ALL_DONE = 0L,
    };

    class ThreadPoolListener {
    public:
        virtual ~ThreadPoolListener() {}
        virtual int32_t OnThreadPoolEvent(ThreadPoolEvent event, void *pEventData) = 0;
    };

    class MWL_API ThreadPool : private NonCopyable {
    public:
        explicit ThreadPool(int32_t maxThreadCount = 0);
        ~ThreadPool();
        int32_t SetMaxThreadCount(int32_t maxThreadCount);
        int32_t GetMaxThreadCount() const;
        int32_t AddTask(ThreadEntry workEntry, void *pWorkData);
        int32_t Cancel();
        int32_t Join();
        int32_t RegisterListener(ThreadPoolListener *pListener);

        struct Implement;
    private:
        Implement *m_pImpl;
    };
}

#endif // __MWL_THREAD_POOL_H__
