#include "inc/BasicDefines.h"

#ifdef __MWL_WIN__

#include "BarrierImplement.h"

namespace mwl {

    Barrier::Implement::Implement(int32_t threshold)
    : _threshold(threshold), _waitingCnt(0), _switch(0) {}

    Barrier::Implement::~Implement() {}

    int32_t Barrier::Implement::_Wait() {
        Mutex::AutoLock l(_m);
        _waitingCnt += 1;
        if (_waitingCnt < _threshold) {
            int32_t s = _switch;
            while (s == _switch) {
                int32_t ret = _c.Wait(_m);
                if (ret < 0) {
                    return ret;
                }
            }
            return 0;
        } else {
            _switch = !_switch;
            _waitingCnt = 0;
            int32_t ret = _c.Broadcast();
            if (ret < 0) {
                return ret;
            } else {
                return 1;
            }
        }
    }

    int32_t Barrier::Implement::_SetThreshold(int32_t threshold) {
        _threshold = threshold;
        return _threshold;
    }
    int32_t Barrier::Implement::_Threshold() {
        return _threshold;
    }
}

#endif // __MWL_WIN__
