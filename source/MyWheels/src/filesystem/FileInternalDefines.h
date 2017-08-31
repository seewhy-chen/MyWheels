#include "inc/BasicDefines.h"
#include "inc/StaticAssert.h"

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
    #define mwl_stat _stat64
    #define mwl_fstat _fstat64
    #define mwl_rmdir _rmdir
    #define mwl_unlink _unlink
#else
    #define mwl_open open
    #define mwl_close close
    #define mwl_lseek lseek
    #define mwl_tell(fhandle) lseek((fhandle), 0, SEEK_CUR)
    #define mwl_read read
    #define mwl_write write
    #define mwl_stat stat
    #define mwl_fstat fstat
    #define mwl_rmdir rmdir
    #define mwl_unlink unlink
#endif

namespace mwl {
    static inline int32_t _MWLFlagsToOSFlags(uint32_t mwlFlags) {
        int32_t osFlags = 0;
        if ((mwlFlags & FF_RDONLY) && (mwlFlags & FF_WRONLY)) {
            osFlags |= O_RDWR;
        } else if (mwlFlags & FF_RDONLY) {
            osFlags |= O_RDONLY;
        } else if (mwlFlags & FF_WRONLY) {
            osFlags |= O_WRONLY;
        }

        if (mwlFlags & FF_CREATE) {
            osFlags |= O_CREAT;
        }
        if (mwlFlags & FF_TRUNC) {
            osFlags |= O_TRUNC;
        }
        if (mwlFlags & FF_APPEND) {
            osFlags |= O_APPEND;
        }
        if (mwlFlags & FF_EXCL) {
            osFlags |= O_EXCL;
        }
#ifdef __MWL_WIN__
        osFlags |= O_BINARY;
#elif defined __MWL_LINUX__
        if (mwlFlags & FF_DIRECTORY) {
            osFlags |= O_DIRECTORY;
        }
        if (mwlFlags & FF_CLOEXEC) {
            osFlags |= O_CLOEXEC;
        }
        if (mwlFlags & FF_NOFOLLOW) {
            osFlags |= O_NOFOLLOW;
        }
#endif
        return osFlags;
    }

    static inline int32_t _MWLModesToOSModes(uint32_t mwlModes) {
        int32_t osModes = 0;
#ifdef __MWL_LINUX__
        if (mwlModes & FM_OWNER_R) {
            osModes |= S_IRUSR;
        }
        if (mwlModes & FM_OWNER_W) {
            osModes |= S_IWUSR;
        }
        if (mwlModes & FM_OWNER_X) {
            osModes |= S_IXUSR;
        }
        if (mwlModes & FM_GROUP_R) {
            osModes |= S_IRGRP;
        }
        if (mwlModes & FM_GROUP_W) {
            osModes |= S_IWGRP;
        }
        if (mwlModes & FM_GROUP_X) {
            osModes |= S_IXGRP;
        }
        if (mwlModes & FM_OTHER_R) {
            osModes |= S_IROTH;
        }
        if (mwlModes & FM_OTHER_W) {
            osModes |= S_IWOTH;
        }
        if (mwlModes & FM_OTHER_X) {
            osModes |= S_IXOTH;
        }
#elif defined __MWL_WIN__
        if ((mwlModes & FM_OWNER_R) || (mwlModes & FM_GROUP_R) || (mwlModes & FM_OTHER_R)) {
            osModes |= _S_IREAD;
        }
        if ((mwlModes & FM_OWNER_W) || (mwlModes & FM_GROUP_W) || (mwlModes & FM_OTHER_W)) {
            osModes |= _S_IWRITE;
        }
        if ((mwlModes & FM_OWNER_X) || (mwlModes & FM_GROUP_X) || (mwlModes & FM_OTHER_X)) {
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

    static inline FileType _StatMode2FileType(uint32_t stMode) {
        if (S_IFDIR & stMode) {
            return FT_DIRECTORY;
        } else if (S_IFREG & stMode) {
            return FT_REG_FILE;
#ifdef __MWL_LINUX__
        } else if (S_IFREG & stMode) {
            return FT_REG_FILE;
#endif
        } else {
            return FT_UNKNOWN;
        }
    }

}
