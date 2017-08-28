#ifndef __MWL_FILE_DEFINES_H__
#define __MWL_FILE_DEFINES_H__

#include "inc/BasicDefines.h"
#include "inc/DateTime.h"

namespace mwl {
    enum FileOpenFlag {
        FOF_RDONLY = 1L << 0,
        FOF_WRONLY = 1L << 1,
        FOF_RDWR = FOF_RDONLY | FOF_WRONLY,
        FOF_CREATE = 1L << 2,
        FOF_TRUNC = 1L << 3,
        FOF_APPEND = 1L << 4,
        FOF_EXCL = 1L << 5,
        FOF_DIRECTORY = 1L << 6,
        FOF_CLOEXEC = 1L << 7,
        FOF_NOFOLLOW = 1L << 8,
    };

    enum FileOpenMode {
        FOM_OWNER_R = 1L << 0,
        FOM_OWNER_W = 1L << 1,
        FOM_OWNER_X = 1L << 2,
        FOM_OWNER_ALL = FOM_OWNER_R | FOM_OWNER_W | FOM_OWNER_X,

        FOM_GROUP_R = 1L << 4,
        FOM_GROUP_W = 1L << 5,
        FOM_GROUP_X = 1L << 6,
        FOM_GROUP_ALL = FOM_GROUP_R | FOM_GROUP_W | FOM_GROUP_X,

        FOM_OTHER_R = 1L << 8,
        FOM_OTHER_W = 1L << 9,
        FOM_OTHER_X = 1L << 10,
        FOM_OTHER_ALL = FOM_OTHER_R | FOM_OTHER_W | FOM_OTHER_X,
    };

    enum FileSeekWhence {
        FRM_BEGIN = 0L,
        FRM_CURR = 1L,
        FRM_END = 2L,

        FileSeekWhenceCount,
    };

    enum FileType {
        UNKNOWN = -1L,
        REG_FILE = 0L,
        DIRECTORY = 1L,
        SYM_LINK = 2L,
    };

    struct FileStat {
        int64_t size;
        DateTime create;
        DateTime access;
        DateTime modified;
    };

}

#endif // __MWL_FILE_DEFINES_H__
