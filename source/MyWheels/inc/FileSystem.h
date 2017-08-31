#ifndef __MWL_FILESYSTEM_H__
#define __MWL_FILESYSTEM_H__

#include "inc/FileDefines.h"
#include "inc/String.h"

namespace mwl {

    class MWL_API FileSystem : private NonCopyable {
    public:
        static bool Exist(const String &path);
        static bool IsFile(const String &path);
        static bool IsDir(const String &path);
        static FileType GetType(const String &path);
        static int64_t GetSize(const String &path);
        static String GetFullPath(const String &path);
        static int32_t Stat(const String &path, FileStat *pStat);
        static int32_t Move(const String &srcPath, const String &dstPath, FileOperatePolicy policy = FOP_SKIP);
        static int32_t DuplicateFile(const String &srcPath, const String &dstPath, FileOperatePolicy policy = FOP_SKIP);
        static int32_t RemoveFile(const String &filePath);
        static int32_t MakeDir(const String &dirPath, uint32_t modes = FM_OWNER_ALL);
        static int32_t ListDir(const String &dirPath, Array<String> *pFiles, Array<String> *pSubDirs = nullptr);
        static int32_t RemoveDir(const String &dirPath);
        static int32_t ParsePath(const String &path, 
            String *pDirName = nullptr, String *pFileName = nullptr, String *pExt = nullptr);
        static String MakePath(const String &dirName, const String &fileName, const String &ext = String::Null());
    };

}

#endif // __MWL_FILESYSTEM_H__
