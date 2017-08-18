#ifndef __MWL_FILESYSTEM_H__
#define __MWL_FILESYSTEM_H__

#include "inc/FileDefines.h"

namespace mwl {

    class MWL_API FileSystem : private NonCopyable {
    public:
        static bool Exist(const char *path);
        static int32_t Stat(const char *path, FileStat *pStat);
        static int32_t CreateTime(const char *path, DateTime *pCreateDate);
        static int32_t AccessTime(const char *path, DateTime *pCreateDate);
        static int32_t ModifiedTime(const char *path, DateTime *pCreateDate);
        static FileType Type(const char *path);


    };

}

#endif // __MWL_FILESYSTEM_H__
