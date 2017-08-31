#include "DateTimeImplement.h"
#include "TimeInternalUtils.h"

namespace mwl {

    static const char *s_defaultFmt = "%x %X";
    DateTime::Implement::Implement(const TimeSpan *fromEpoch, bool isUTC)
        : _utc(isUTC), _strFmt(s_defaultFmt) {
        if (fromEpoch) {
            _tv.tv_sec = static_cast<long>(fromEpoch->ToI64(SECOND));
        } else {
            _tv.tv_sec = 0;
        }
        _tv.tv_usec = 0;
        _Tv2Tm(&_tv, &_tm, _utc);
    }

    DateTime::Implement::Implement(const Implement *pSrc) {
        _tv = pSrc->_tv;
        _tm = pSrc->_tm;
        _utc = pSrc->_utc;
    }

    int32_t DateTime::Implement::_FromStr(const String &timeString, const String &fmt) {
        memset(&_tm, 0, sizeof(_tm));
        memset(&_tv, 0, sizeof(_tv));
        char *pEnd = strptime(timeString.C_Str(), fmt.C_Str(), &_tm);
        _Tm2Tv(&_tm, &_tv, _utc);
        if (!pEnd) {
            return 0;
        } else {
            return static_cast<int32_t>(pEnd - timeString.C_Str());
        }
    }
    
    void DateTime::Implement::_SetStrFmt(const String &strFmt) {
        _strFmt = strFmt.Empty() ? s_defaultFmt : strFmt;
    }
}
