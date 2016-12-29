#ifndef __MWL_THREAD_H__
#define __MWL_THREAD_H__

#include "BasicDefines.h"

namespace mwl {

    struct MWL_API ThreadID {
        ThreadID();
        ThreadID(const ThreadID &rhs);
        ThreadID& operator=(const ThreadID &rhs);
        ~ThreadID();

        uint32_t PID() const;
        uint32_t TID() const;
        bool operator==(const ThreadID &rhs) const;

        struct Implement;
        Implement *m_pImpl;
    };

    class MWL_API Thread : private NonCopyable {
    public:
        Thread();
        ~Thread();
        
        void SetTag(const char *tag);
        virtual int32_t Entry() = 0;

        int32_t Start(int32_t timeoutInMs = -1);
        int32_t Stop(int32_t timeoutInMs = -1);
        int32_t Join(int32_t timeoutInMs = -1);
        void QueryToStop();

        const ThreadID& Parent() const;
        const ThreadID& Self() const;
        const char *Tag() const;
        bool IsRunning() const;
        int32_t ExitCode() const;

    protected:
        bool StopQueried();

    public:
        struct Implement;
    private:
        Implement *m_pImpl;
    };

}

#endif
