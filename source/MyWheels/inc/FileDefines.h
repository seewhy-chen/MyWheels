#ifndef __MWL_FILE_DEFINES_H__
#define __MWL_FILE_DEFINES_H__

#include "inc/BasicDefines.h"
#include "inc/DateTime.h"

namespace mwl {
    enum FileOpenFlag {
        FF_RDONLY = 1L << 0,
        FF_WRONLY = 1L << 1,
        FF_RDWR = FF_RDONLY | FF_WRONLY,
        FF_CREATE = 1L << 3,
        FF_TRUNC = 1L << 4,
        FF_APPEND = 1L << 5,
        FF_EXCL = 1L << 6,
        FF_DIRECTORY = 1L << 7,
        FF_CLOEXEC = 1L << 8,
        FF_NOFOLLOW = 1L << 9,
    };

    enum FileOpenMode {
        FM_OWNER_R = 1L << 0,
        FM_OWNER_W = 1L << 1,
        FM_OWNER_X = 1L << 2,
        FM_OWNER_ALL = FM_OWNER_R | FM_OWNER_W | FM_OWNER_X,

        FM_GROUP_R = 1L << 4,
        FM_GROUP_W = 1L << 5,
        FM_GROUP_X = 1L << 6,
        FM_GROUP_ALL = FM_GROUP_R | FM_GROUP_W | FM_GROUP_X,

        FM_OTHER_R = 1L << 8,
        FM_OTHER_W = 1L << 9,
        FM_OTHER_X = 1L << 10,
        FM_OTHER_ALL = FM_OTHER_R | FM_OTHER_W | FM_OTHER_X,
    };

    enum FileSeekWhence {
        FRM_BEGIN = 0L,
        FRM_CURR = 1L,
        FRM_END = 2L,

        FileSeekWhenceCount,
    };

    enum FileType {
        FT_UNKNOWN = -1L,
        FT_REG_FILE = 0L,
        FT_DIRECTORY = 1L,
        FT_SYM_LINK = 2L,
    };

    enum FileOperatePolicy {
        FOP_SKIP,
        FOP_OVERWRITE,
        FOP_ONESHOT,
        FOP_RECURSIVELY,
    };

    struct FileStat {
        FileStat() : size(0), type(FT_UNKNOWN) {}
        String fullpath;
        int64_t size;
        FileType type;
        DateTime create;
        DateTime access;
        DateTime modified;
    };

}

#endif // __MWL_FILE_DEFINES_H__
