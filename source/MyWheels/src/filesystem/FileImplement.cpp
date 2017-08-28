#include "FileImplement.h"

#ifdef __MWL_WIN__
#include <io.h>
#endif

#ifdef __MWL_WIN__
#define mwl_open _open
#define mwl_close _close
#define mwl_lseek _lseeki64
#define mwl_tell _telli64
#else
#define mwl_open open
#define mwl_close close
#define mwl_lseek lseek
#define mwl_tell(fhandle) lseek((fhandle), 0, SEEK_CUR)
#endif
namespace mwl {

    File::Implement::Implement() : _fh(-1) {}

    File::Implement::~Implement() {
        _Close();
    }

    static inline int32_t _MWLFlagsToOSFlags(uint32_t mwlFlags) {
        return 0;
    }

    static inline int32_t _MWLModesToOSModes(uint32_t mwlModes) {
        return 0;
    }

    static inline int32_t _MWLWhenceToOSWhence(FileSeekWhence mwlWhence) {
        static const int32_t s_osWhence[] = {
            SEEK_SET, SEEK_CUR, SEEK_END,
        };
        MWL_STATIC_ASSERT(MWL_ARR_SIZE(s_osWhence) == FileSeekWhenceCount, file_whence_count_mismatch);
        return s_osWhence[mwlWhence];
    }

    int32_t File::Implement::_Open(const String &path, uint32_t flags, uint32_t modes) {
        _Close();
        _fh = mwl_open(path.C_Str(), _MWLFlagsToOSFlags(flags), _MWLModesToOSModes(modes));
        return _fh;
    }

    int32_t File::Implement::_Close() {
        int32_t ret = 0;
        if (_fh >= 0) {
            ret = mwl_close(_fh);
            _fh = -1;
        }
        return ret;
    }

    int32_t File::Implement::_Read(ByteArray &data, int32_t len) {
        return -1;
    }

    int32_t File::Implement::_Read(void *pData, int32_t len) {
        return -1;
    }

    int32_t File::Implement::_Write(const ByteArray &data, int32_t len) {
        return -1;
    }

    int32_t File::Implement::_Write(const void *pData, int32_t len) {
        return -1;
    }

    int64_t File::Implement::_Size() {
        return -1;
    }

    int64_t File::Implement::_Tell() {
        return mwl_tell(_fh);
    }

    int64_t File::Implement::_Seek(int64_t offset, FileSeekWhence whence) {
        mwl_lseek(_fh, offset, _MWLWhenceToOSWhence(whence));
        return -1;
    }

    int32_t File::Implement::_Stat(FileStat *pStat) {
        return -1;
    }

}
