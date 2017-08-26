#include "DateTimeImplement.h"
#include "TimeInternalUtils.h"

namespace mwl {


    DateTime::Implement::Implement(bool isUTC)
    : _utc(isUTC), _defaultFmt(s_defaultFmt) {
        memset(&_tm, 0, sizeof(_tm));
        memset(&_tv, 0, sizeof(_tv));
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
}
