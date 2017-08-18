#ifndef __MWL_FILE_H__
#define __MWL_FILE_H__

#include "inc/FileDefines.h"
#include "inc/String.h"
#include "inc/ByteArray.h"

namespace mwl {

    class MWL_API File : private NonCopyable {
    public:
        File();
        ~File();
        int32_t Open(const String &path, uint32_t flags, uint32_t mode);
        int32_t Close();
        int32_t Read(ByteArray &data, int32_t len);
        int32_t Read(void *pData, int32_t len);
        int32_t Write(const ByteArray &data, int32_t len = -1);
        int32_t Write(const void *pData, int32_t len);

        int64_t Size() const;
        int64_t Tell() const;
        int64_t Seek(int64_t offset, FileSeekWhence whence = FRM_BEGIN);
        int32_t Stat(FileStat *pStat);

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_FILE_H__
