#ifndef __MWL_FILESYSTEM_H__
#define __MWL_FILESYSTEM_H__

#include "inc/FileDefines.h"
#include "inc/String.h"

namespace mwl {

    class MWL_API FileSystem : private NonCopyable {
    public:
        static bool Exist(const String &path);
        static int32_t Stat(const String &path, FileStat *pStat);
        static int32_t CreateTime(const String &path, DateTime *pCreateDate);
        static int32_t AccessTime(const String &path, DateTime *pCreateDate);
        static int32_t ModifiedTime(const String &path, DateTime *pCreateDate);
        static FileType Type(const String &path);
    };

}

#endif // __MWL_FILESYSTEM_H__
