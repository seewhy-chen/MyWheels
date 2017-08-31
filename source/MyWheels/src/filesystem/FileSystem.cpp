#include "inc/FileSystem.h"
#include "inc/File.h"
#include "FileInternalDefines.h"

#ifdef __MWL_WIN__
#include <direct.h>
#else
#include <dirent.h>
#endif

namespace mwl {

    static inline const String & _PathSeperator() {
        static String pathSep("/");
        return pathSep;
    }

    static inline const String & _ExtSeperator() {
        static String extSep(".");
        return extSep;
    }

    bool FileSystem::Exist(const String &path) {
        struct mwl_stat s;
        int32_t ret = mwl_stat(path.C_Str(), &s);
        return ret >= 0;
    }

    bool FileSystem::IsFile(const String &path) {
        struct mwl_stat s;
        int32_t ret = mwl_stat(path.C_Str(), &s);
        if (ret >= 0) {
            return S_IFREG & s.st_mode;
        } else {
            return false;
        }
    }

    bool FileSystem::IsDir(const String &path) {
        struct mwl_stat s;
        int32_t ret = mwl_stat(path.C_Str(), &s);
        if (ret >= 0) {
            return S_IFDIR & s.st_mode;
        } else {
            return false;
        }
    }

    FileType FileSystem::GetType(const String &path) {
        struct mwl_stat s;
        int32_t ret = mwl_stat(path.C_Str(), &s);
        if (ret >= 0) {
            return _StatMode2FileType(s.st_mode);
        } else {
            return FT_UNKNOWN;
        }
    }

    int64_t FileSystem::GetSize(const String &path) {
        struct mwl_stat s;
        int32_t ret = mwl_stat(path.C_Str(), &s);
        if (ret >= 0) {
            return s.st_size;
        } else {
            return ret;
        }
    }

    String FileSystem::GetFullPath(const String &path) {
        String result;
#ifdef __MWL_WIN__
        if (!Exist(path)) {
            return String();
        }
        ByteArray pathBuf(128);
        result = _fullpath(reinterpret_cast<char*>(pathBuf.Data()), path.C_Str(), pathBuf.Size());
        while (result.Empty()) {
            pathBuf.Increase(64);
            result = _fullpath(reinterpret_cast<char*>(pathBuf.Data()), path.C_Str(), pathBuf.Size());
        }
        result.Replace("\\", _PathSeperator());
#else
        char *fullPath = realpath(path.C_Str(), nullptr);
        result = fullPath;
        free(fullPath);
#endif
        return result;
    }

    int32_t FileSystem::Stat(const String &path, FileStat *pStat) {
        if (!pStat) {
            return -1;
        }

        struct mwl_stat s;
        int32_t ret = mwl_stat(path.C_Str(), &s);
        if (ret >= 0) {
            pStat->fullpath = GetFullPath(path);
            pStat->create = DateTime(TimeSpan(static_cast<int64_t>(s.st_ctime), SECOND), DateTime::TZ_UTC);
            pStat->modified = DateTime(TimeSpan(static_cast<int64_t>(s.st_mtime), SECOND), DateTime::TZ_UTC);
            pStat->access = DateTime(TimeSpan(static_cast<int64_t>(s.st_atime), SECOND), DateTime::TZ_UTC);
            pStat->type = _StatMode2FileType(s.st_mode);
            pStat->size = s.st_size;
        }
        return ret;
    }

    int32_t FileSystem::DuplicateFile(const String &srcPath, const String &dstPath, FileOperatePolicy policy) {
        if (!IsFile(srcPath)) {
            return -EINVAL;
        }
#ifdef __MWL_WIN__
        if (!CopyFile(srcPath.C_Str(), dstPath.C_Str(), policy == FOP_SKIP)) {
            return -static_cast<int32_t>(GetLastError());
        }
        return static_cast<int32_t>(GetSize(dstPath));
#else
        File srcFile(srcPath);
        if (!srcFile.Readable()) {
            return -srcFile.Errno();;
        }

        String dstFilePath = dstPath;
        if (IsDir(dstPath)) {
            String baseName;
            ParsePath(srcPath, nullptr, &baseName);
            dstFilePath = MakePath(dstPath, baseName);
        }
        if (dstFilePath == srcPath) {
            return -EEXIST;
        }
        if (Exist(dstFilePath) && policy != FOP_OVERWRITE) {
            return -EEXIST;
        }

        File dstFile;
        dstFile.Create(dstFilePath);
        if (!dstFile.Writable()) {
            return -dstFile.Errno();
        }

        ByteArray buf(4096);
        int32_t bytesRead = srcFile.Read(buf, buf.Size());
        int32_t ret = 0;
        while (bytesRead > 0) {
            int32_t bytesWritten = dstFile.Write(buf, bytesRead);
            if (bytesWritten < 0) {
                MWL_WARN_ERRNO("copy %s to %s failed", dstFile.Errno(), srcPath.C_Str(), dstFilePath.C_Str());
                dstFile.Close();
                RemoveFile(dstFilePath);
                ret = -dstFile.Errno();
                break;
            }
            ret += bytesWritten;
            if (bytesWritten != bytesRead) {
                break;
            }
            bytesRead = srcFile.Read(buf, buf.Size());
        }
        return ret;
#endif
    }

    int32_t FileSystem::Move(const String &srcPath, const String &dstPath, FileOperatePolicy policy) {
        if (srcPath == dstPath) {
            return 0;
        }
#ifdef __MWL_WIN__
        uint32_t flags = MOVEFILE_COPY_ALLOWED;
        if (FOP_OVERWRITE == policy) {
            flags |= MOVEFILE_REPLACE_EXISTING;
        }
        return MoveFileEx(srcPath.C_Str(), dstPath.C_Str(), flags) ? 0 : -1;
#else
        if (policy != FOP_OVERWRITE && Exist(dstPath)) {
            return -EEXIST;
        }
        int32_t ret = rename(srcPath.C_Str(), dstPath.C_Str());
        return ret >= 0 ? ret : -errno;
#endif
    }

    int32_t FileSystem::RemoveFile(const String &filePath) {
        int32_t ret = mwl_unlink(filePath.C_Str());
        return ret >= 0 ? ret : -errno;
    }

    int32_t FileSystem::MakeDir(const String &dirPath, uint32_t modes) {
        if (IsDir(dirPath)) {
            return 0;
        }
#ifdef __MWL_WIN__
        int32_t ret = _mkdir(dirPath.C_Str());
#else
        int32_t ret = mkdir(dirPath.C_Str(), _MWLModesToOSModes(modes));
#endif
        return ret >= 0 ? ret : -errno;
    }
    
    int32_t FileSystem::RemoveDir(const String &dirPath) {
        int32_t ret = mwl_rmdir(dirPath.C_Str());
        return ret >= 0 ? ret : -errno;
    }

    int32_t FileSystem::ListDir(const String &dirpath, Array<String> *pFiles, Array<String> *pSubDirs) {
        if (!IsDir(dirpath)) {
            return -1;
        }

        String searchPath = GetFullPath(dirpath);
        if (!searchPath.EndsWith(_PathSeperator())) {
            searchPath += _PathSeperator();
        }
        String currDir(".");
        String parentDir("..");
#ifdef __MWL_WIN__
        searchPath += "/*";
        WIN32_FIND_DATA fData;
        HANDLE hFind = FindFirstFile(searchPath.C_Str(), &fData);
        if (INVALID_HANDLE_VALUE == hFind) {
            return -1;
        }
        do {
            if (currDir == fData.cFileName || parentDir == fData.cFileName) {
                continue;
            }
            if ((fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && pSubDirs) {
                pSubDirs->Append(fData.cFileName);
            } else if (pFiles) {
                pFiles->Append(fData.cFileName);
            }
        } while (FindNextFile(hFind, &fData));
        FindClose(hFind);
#else
        DIR *pDir = opendir(searchPath.C_Str());
        if (pDir) {
            struct dirent entry;
            struct dirent *result = nullptr;
            errno = 0;
            readdir_r(pDir, &entry, &result);
            while (result) {
                if (currDir != result->d_name && parentDir != result->d_name) {
                    if (DT_UNKNOWN == result->d_type) {
                        FileType ft = GetType(searchPath + result->d_name);
                        if ((FT_DIRECTORY == ft) && pSubDirs) {
                            pSubDirs->Append(result->d_name);
                        } else if ((FT_REG_FILE == ft) && pFiles) {
                            pFiles->Append(result->d_name);
                        }
                    } else if ((result->d_type & DT_DIR) && pSubDirs) {
                        pSubDirs->Append(result->d_name);
                    } else if (((result->d_type & DT_REG) || (result->d_type & DT_LNK)) && pFiles) {
                        pFiles->Append(result->d_name);
                    }
                }
                readdir_r(pDir, &entry, &result);
            }
            if (errno) {
                MWL_WARN_ERRNO("read directory %s failed", errno, searchPath.C_Str());
            }
            closedir(pDir);
        } else {
            MWL_WARN_ERRNO("open directory %s failed", errno, searchPath.C_Str());
        }
#endif
        return 0;
   }

    int32_t FileSystem::ParsePath(const String &path, String *pDirName, String *pBaseName) {
        if (!pDirName && !pBaseName) {
            return 0;
        }

        Array<String> splitted;
        if (path.Contains("\\")) {
            splitted = path.Replaced("\\", _PathSeperator()).RSplit(_PathSeperator(), 1);
        } else {
            splitted = path.RSplit(_PathSeperator(), 1);
        }
        if (pDirName) {
            *pDirName = splitted[0];
        }
        if (splitted.Size() > 1 && pBaseName) {
            *pBaseName = splitted[1];
        }
        return 0;
    }

    int32_t FileSystem::ParsePath(const String &path, String *pDirName, String *pFileName, String *pExt) {
        if (!pDirName && !pFileName && !pExt) {
            return 0;
        }

        String dirName, baseName;
        int32_t ret = ParsePath(path, &dirName, &baseName);
        if (ret < 0) {
            return ret;
        }

        if (pDirName) {
            *pDirName = dirName;
        }
        Array<String> splitted = baseName.RSplit(_ExtSeperator(), 1);
        if (pFileName) {
            *pFileName = splitted[0];
        }
        if (pExt && splitted.Size() > 1) {
            *pExt = splitted[1];
        }
        return 0;
    }

    String FileSystem::MakePath(const String &dirName, const String &fileName, const String &ext) {
        String path(dirName);
        if (fileName.Empty()) {
            return path;
        }

        if (path.Empty()) {
            path = "./";
        }
        if (!path.EndsWith(_PathSeperator())) {
            path += _PathSeperator();
        }
        path += fileName;
        if (!ext.Empty()) {
            if (ext.StartsWith(_ExtSeperator())) {
                path += ext;
            } else {
                path += _ExtSeperator();
                path += ext;
            }
        }
        return path;
    }

    String FileSystem::MakePath(const String &dirName, const String &baseName) {
        String path(dirName);
        if (path.Empty()) {
            path = "./";
        }
        if (!path.EndsWith(_PathSeperator())) {
            path += _PathSeperator();
        }
        return path + baseName;
    }
}
