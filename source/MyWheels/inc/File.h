#ifndef __MWL_FILE_H__
#define __MWL_FILE_H__

#include "inc/FileDefines.h"
#include "inc/String.h"
#include "inc/ByteArray.h"

namespace mwl {

    class MWL_API File : private NonCopyable {
    public:
        File();
        File(const String &path, uint32_t flags = FOF_RDONLY, uint32_t modes = 0);
        ~File();
        int32_t Open(const String &path, uint32_t flags = FOF_RDONLY, uint32_t modes = 0);
        int32_t Create(const String &path, uint32_t modes = FOM_OWNER_ALL);
        int32_t Close();
        int32_t Read(ByteArray &data, int32_t len = -1);
        int32_t Read(void *pData, int32_t len);
        int32_t Write(const ByteArray &data, int32_t len = -1);
        int32_t Write(const void *pData, int32_t len);

        int64_t Seek(int64_t offset, FileSeekWhence whence = FRM_BEGIN);
        bool Opened() const;
        bool Readable() const;
        bool Writable() const;
        const String Pathname() const;
        const String Dirname() const;
        const String Basename() const;
        const String Filename() const;
        const String Extname() const;
        const String DotExtname() const;

        int64_t Size() const;
        int64_t Tell() const;
        int32_t CreateDateTime(DateTime *pDate) const;
        int32_t ModifyDateTime(DateTime *pDate) const;
        int32_t AccessDateTime(DateTime *pDate) const;
        int32_t Stat(FileStat *pStat) const;


    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_FILE_H__
