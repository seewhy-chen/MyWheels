#include "FileImplement.h"
#include "../datetime/DateTimeImplement.h"

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef __MWL_WIN__
#include <io.h>
#else
#include <unistd.h>
#endif


#ifdef __MWL_WIN__
    #define mwl_open _open
    #define mwl_close _close
    #define mwl_lseek _lseeki64
    #define mwl_tell _telli64
    #define mwl_read _read
    #define mwl_write _write
    #define mwl_stat  struct _stat64
    #define mwl_fstat  _fstat64
#else
    #define mwl_open open
    #define mwl_close close
    #define mwl_lseek lseek
    #define mwl_tell(fhandle) lseek((fhandle), 0, SEEK_CUR)
    #define mwl_read read
    #define mwl_write write
    #define mwl_stat  struct stat
    #define mwl_fstat  fstat
#endif

namespace mwl {

    File::Implement::Implement() : _fh(-1), _flags(0), _modes(0) {}

    File::Implement::~Implement() {
        _Close();
    }

    static inline int32_t _MWLFlagsToOSFlags(uint32_t mwlFlags) {
        int32_t osFlags = 0;
        if ((mwlFlags & FOF_RDONLY) && (mwlFlags & FOF_WRONLY)) {
            osFlags |= O_RDWR;
        } else if (mwlFlags & FOF_RDONLY) {
            osFlags |= O_RDONLY;
        } else if (mwlFlags & FOF_WRONLY) {
            osFlags |= O_WRONLY;
        }

        if (mwlFlags & FOF_CREATE) {
            osFlags |= O_CREAT;
        }
        if (mwlFlags & FOF_TRUNC) {
            osFlags |= O_TRUNC;
        }
        if (mwlFlags & FOF_APPEND) {
            osFlags |= O_APPEND;
        }
        if (mwlFlags & FOF_EXCL) {
            osFlags |= O_EXCL;
        }
#ifdef __MWL_WIN__
        osFlags |= O_BINARY;
#elif defined __MWL_LINUX__
        if (mwlFlags & FOF_DIRECTORY) {
            osFlags |= O_DIRECTORY;
        }
        if (mwlFlags & FOF_CLOEXEC) {
            osFlags |= O_CLOEXEC;
        }
        if (mwlFlags & FOF_NOFOLLOW) {
            osFlags |= O_NOFOLLOW;
        }
#endif
        return osFlags;
    }

    static inline int32_t _MWLModesToOSModes(uint32_t mwlModes) {
        int32_t osModes = 0;
#ifdef __MWL_LINUX__
        if (mwlModes & FOM_OWNER_R) {
            osModes |= S_IRUSR;
        }
        if (mwlModes & FOM_OWNER_W) {
            osModes |= S_IWUSR;
        }
        if (mwlModes & FOM_OWNER_X) {
            osModes |= S_IXUSR;
        }
        if (mwlModes & FOM_GROUP_R) {
            osModes |= S_IRGRP;
        }
        if (mwlModes & FOM_GROUP_W) {
            osModes |= S_IWGRP;
        }
        if (mwlModes & FOM_GROUP_X) {
            osModes |= S_IXGRP;
        }
        if (mwlModes & FOM_OTHER_R) {
            osModes |= S_IROTH;
        }
        if (mwlModes & FOM_OTHER_W) {
            osModes |= S_IWOTH;
        }
        if (mwlModes & FOM_OTHER_X) {
            osModes |= S_IXOTH;
        }
#elif defined __MWL_WIN__
        if ((mwlModes & FOM_OWNER_R) || (mwlModes & FOM_GROUP_R) || (mwlModes & FOM_OTHER_R)) {
            osModes |= _S_IREAD;
        }
        if ((mwlModes & FOM_OWNER_W) || (mwlModes & FOM_GROUP_W) || (mwlModes & FOM_OTHER_W)) {
            osModes |= _S_IWRITE;
        }
        if ((mwlModes & FOM_OWNER_X) || (mwlModes & FOM_GROUP_X) || (mwlModes & FOM_OTHER_X)) {
            osModes |= _S_IEXEC;
        }
#endif

        return osModes;
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
        if (_fh >= 0) {
            _flags = flags;
            _modes = modes;
            char absPath[4096] = {0};
#ifdef __MWL_WIN__
            _filePath = _fullpath(absPath, path.C_Str(), sizeof(absPath));
#else
            _filePath = realpath(path.C_Str(), absPath);
#endif
            _filePath.Replace("\\", "/");
        } else {
            _err = errno;
        }
        return _fh;
    }

    int32_t File::Implement::_Close() {
        int32_t ret = 0;
        if (_fh >= 0) {
            ret = mwl_close(_fh);
            if (ret < 0) {
                _err = errno;
            } else {
                _fh = -1;
                _flags = 0;
                _modes = 0;
                _filePath.Clear();
            }
        }
        return ret;
    }

    int32_t File::Implement::_Read(ByteArray &data, int32_t len) {
        if (len <= 0) {
            return 0;
        }
        data.Resize(len);
        return mwl_read(_fh, data.Data(), len);
    }

    int32_t File::Implement::_Read(void *pData, int32_t len) {
        if (!pData || len <= 0) {
            return 0;
        }
        return mwl_read(_fh, pData, len);
    }

    int32_t File::Implement::_Write(const ByteArray &data, int32_t len) {
        if (len <= 0) {
            return 0;
        }
        return mwl_write(_fh, data.Data(), len);
    }

    int32_t File::Implement::_Write(const void *pData, int32_t len) {
        if (!pData || len <= 0) {
            return 0;
        }
        return mwl_write(_fh, pData, len);
    }

    bool File::Implement::_Opened() {
        return _fh >= 0;
    }

    bool File::Implement::_Readable() {
        return _Opened() && (_flags & FOF_RDONLY);
    }

    bool File::Implement::_Writable() {
        return _Opened() && (_flags & FOF_WRONLY);
    }

    String File::Implement::_Pathname() {
        return _filePath;
    }

    String File::Implement::_Dirname() {
        return _filePath.RSplit("/", 1)[0];
    }

    String File::Implement::_Basename() {
        return _filePath.RSplit("/", 1)[-1];
    }

    String File::Implement::_Filename() {
        return _Basename().RSplit(".", 1)[0];
    }

    String File::Implement::_Extname() {
        Array<String> splitted = _filePath.RSplit(".", 1);
        if (splitted.Size() > 2) {
            return splitted[-1];
        } else {
            return String();
        }
    }

    String File::Implement::_DotExtname() {
        String ext = _Extname();
        if (!ext.Empty()) {
            ext = "." + ext;
        }
        return ext;
    }

    int64_t File::Implement::_Size() {
        mwl_stat fStat;
        int32_t ret = mwl_fstat(_fh, &fStat);
        if (ret >= 0) {
            return fStat.st_size;
        } else {
            _err = errno;
            return -1;
        }
    }

    int64_t File::Implement::_Tell() {
        return mwl_tell(_fh);
    }

    int64_t File::Implement::_Seek(int64_t offset, FileSeekWhence whence) {
        return mwl_lseek(_fh, offset, _MWLWhenceToOSWhence(whence));
    }

    int32_t File::Implement::_Stat(FileStat *pStat) {
        if (!pStat) {
            _err = EINVAL;
            return -1;
        }
        mwl_stat fStat;
        int32_t ret = mwl_fstat(_fh, &fStat);
        if (ret >= 0) {
            pStat->create = DateTime(DateTime::TZ_UTC) + TimeSpan(static_cast<int64_t>(fStat.st_ctime), SECOND);
            pStat->modified = DateTime(DateTime::TZ_UTC) + TimeSpan(static_cast<int64_t>(fStat.st_mtime), SECOND);
            pStat->access = DateTime(DateTime::TZ_UTC) + TimeSpan(static_cast<int64_t>(fStat.st_atime), SECOND);
            pStat->size = fStat.st_size;
        } else {
            _err = errno;
        }
        return ret;
    }

}
