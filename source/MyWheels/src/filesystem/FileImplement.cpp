#include "inc/FileSystem.h"
#include "FileImplement.h"
#include "../datetime/DateTimeImplement.h"

namespace mwl {

    File::Implement::Implement() : _fh(-1), _flags(0), _modes(0), _err(0) {}

    File::Implement::~Implement() {
        _Close();
    }

    int32_t File::Implement::_Open(const String &path, uint32_t flags, uint32_t modes) {
        int32_t ret = _Close();
        if (ret < 0) {
            return ret;
        }
        _fh = mwl_open(path.C_Str(), _MWLFlagsToOSFlags(flags), _MWLModesToOSModes(modes));
        if (_fh >= 0) {
            _flags = flags;
            _modes = modes;
            _err = 0;
            _filePath = FileSystem::GetFullPath(path);
            _filePath.Replace("\\", "/");
            FileSystem::ParsePath(_filePath, &_dirName, &_fileName, &_extName);
            _baseName = _fileName + _extName;
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
                _dirName.Clear();
                _fileName.Clear();
                _extName.Clear();
                _baseName.Clear();
            }
        }
        return ret;
    }

    int32_t File::Implement::_Read(ByteArray &data, int32_t len) {
        if (len < 0) {
            _err = EINVAL;
            return -1;
        }
        data.Resize(len);
        int32_t ret = mwl_read(_fh, data.Data(), len);
        if (ret < 0) {
            _err = errno;
        }
        return ret;
    }

    int32_t File::Implement::_Read(void *pData, int32_t len) {
        if (len < 0) {
            _err = EINVAL;
            return -1;
        }

        int32_t ret = mwl_read(_fh, pData, len);
        if (ret < 0) {
            _err = errno;
        }
        return ret;
    }

    int32_t File::Implement::_Write(const ByteArray &data, int32_t len) {
        if (len < 0) {
            _err = EINVAL;
            return -1;
        }

        int32_t ret = mwl_write(_fh, data.Data(), len);
        if (ret < 0) {
            _err = errno;
        }
        return ret;
    }

    int32_t File::Implement::_Write(const void *pData, int32_t len) {
        if (len < 0) {
            _err = EINVAL;
            return -1;
        }

        int32_t ret = mwl_write(_fh, pData, len);
        if (ret < 0) {
            _err = errno;
        }
        return ret;
    }

    bool File::Implement::_Opened() {
        return _fh >= 0;
    }

    bool File::Implement::_Readable() {
        return _Opened() && (_flags & FF_RDONLY);
    }

    bool File::Implement::_Writable() {
        return _Opened() && (_flags & FF_WRONLY);
    }

    String File::Implement::_PathName() {
        return _filePath;
    }

    String File::Implement::_DirName() {
        return _dirName;
    }

    String File::Implement::_FileName() {
        return _fileName;
    }

    String File::Implement::_ExtName() {
        return _extName;
    }

    String File::Implement::_BaseName() {
        return _baseName;
    }

    int64_t File::Implement::_Size() {
        struct mwl_stat fStat;
        int32_t ret = mwl_fstat(_fh, &fStat);
        if (ret >= 0) {
            return fStat.st_size;
        } else {
            _err = errno;
            return ret;
        }
    }

    int64_t File::Implement::_Tell() {
         int64_t ret = mwl_tell(_fh);
         if (ret < 0) {
             _err = errno;
         }
         return ret;
    }

    int64_t File::Implement::_Seek(int64_t offset, FileSeekWhence whence) {
        int64_t ret = mwl_lseek(_fh, offset, _MWLWhenceToOSWhence(whence));
        if (ret < 0) {
            _err = errno;
        }
        return ret;
    }

    int32_t File::Implement::_Stat(FileStat *pStat) {
        if (!pStat) {
            _err = EINVAL;
            return -1;
        }
        struct mwl_stat fStat;
        int32_t ret = mwl_fstat(_fh, &fStat);
        if (ret >= 0) {
            pStat->create = DateTime(TimeSpan(static_cast<int64_t>(fStat.st_ctime), SECOND), DateTime::TZ_UTC);
            pStat->modified = DateTime(TimeSpan(static_cast<int64_t>(fStat.st_mtime), SECOND), DateTime::TZ_UTC);
            pStat->access = DateTime(TimeSpan(static_cast<int64_t>(fStat.st_atime), SECOND), DateTime::TZ_UTC);
            pStat->type = _StatMode2FileType(fStat.st_mode);
            pStat->size = fStat.st_size;
        } else {
            _err = errno;
        }
        return ret;
    }

    int32_t File::Implement::_Errno() {
        return _err;
    }
}
