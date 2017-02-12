#ifndef __MWL_THREAD_IMPLEMENT_H__
#define __MWL_THREAD_IMPLEMENT_H__

#include "inc/Thread.h"
#include "inc/Mutex.h"
#include "inc/Condition.h"

#include "InternalCommon.h"
#include "ThreadImplement.h"
#include "ThreadContextImplement.h"

#include <string>

namespace mwl {

    struct Thread::Implement {
        Implement() {
#ifdef __MWL_WIN__
        threadHdl = 0;
#endif
        }

        ~Implement() {
            _Stop(-1);
        }

        inline int32_t _SetTag(const char* tag) {
            if (tag) {
                context.m_pImpl->tag = tag;
            } else {
                context.m_pImpl->tag.clear();
            }
            return ERR_NONE;
        }

        int32_t _Stop(const TimeSpec &timeout) {
            if (!_IsRunning()) {
                return 0;
            }
            _QueryToStop();
            return _Join(timeout);
        }

        inline void _QueryToStop() {
            return context.QueryToStop();
        }

        inline void *_SharedData() {
            return context.SharedData();
        }

        inline const ThreadID &_ParentID() {
            return context.ParentID();
        }

        inline const ThreadID &_SelfID() {
            return context.SelfID();
        }

        inline const char *_Tag() {
            return context.Tag();
        }

        inline bool _IsRunning() {
            Mutex::AutoLock _l(context.m_pImpl->lock);
            return context.m_pImpl->isRunning;
        }

        inline int32_t _ExitCode() {
            Mutex::AutoLock _l(context.m_pImpl->lock);
            if (context.m_pImpl->isRunning) {
                return ERR_INVAL_OP;
            } else {
                return context.m_pImpl->exitCode;
            }
        }

        int32_t _Start(ThreadEntry entry, void *pSharedData, const TimeSpec &timeout);
        int32_t _Join(const TimeSpec &timeout);

        ThreadContext context;
        ThreadEntry entry;
#ifdef __MWL_WIN__
        HANDLE threadHdl;
#endif
    };

}

#endif // __MWL_THREAD_IMPLEMENT_H__