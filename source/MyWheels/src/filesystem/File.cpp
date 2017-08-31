#include "inc/File.h"
#include "FileImplement.h"

namespace mwl {
    File::File() : m_pImpl(new Implement()) {}

    File::File(const String &path, uint32_t flags, uint32_t modes)
        : m_pImpl(new Implement()) {
        if (!path.Empty()) {
            m_pImpl->_Open(path, flags, modes);
        }
    }

    File::~File() {
        delete m_pImpl;
    }

    int32_t File::Open(const String &path, uint32_t flags, uint32_t modes) {
        return m_pImpl->_Open(path, flags, modes);
    }

    int32_t File::Create(const String &path, uint32_t modes) {
        return m_pImpl->_Open(path, FF_WRONLY | FF_CREATE | FF_TRUNC, modes);
    }

    int32_t File::Close() {
        return m_pImpl->_Close();
    }

    int32_t File::Read(ByteArray &data, int32_t len) {
        return m_pImpl->_Read(data, len);
    }

    int32_t File::Read(void *pData, int32_t len) {
        return m_pImpl->_Read(pData, len);
    }

    int32_t File::Write(const ByteArray &data, int32_t len) {
        return m_pImpl->_Write(data, len);
    }

    int32_t File::File::Write(const void *pData, int32_t len) {
        return m_pImpl->_Write(pData, len);
    }

    bool File::Opened() const {
        return m_pImpl->_Opened();
    }

    bool File::Readable() const {
        return m_pImpl->_Readable();
    }

    bool File::Writable() const {
        return m_pImpl->_Writable();
    }

    const String File::PathName() const {
        return m_pImpl->_PathName();
    }

    const String File::DirName() const {
        return m_pImpl->_DirName();
    }

    const String File::BaseName() const {
        return m_pImpl->_BaseName();
    }

    const String File::FileName() const {
        return m_pImpl->_FileName();
    }

    const String File::ExtName() const {
        return m_pImpl->_ExtName();
    }

    int64_t File::Size() const {
        return m_pImpl->_Size();
    }

    int64_t File::Tell() const {
        return m_pImpl->_Tell();
    }

    int64_t File::Seek(int64_t offset, FileSeekWhence whence) {
        return m_pImpl->_Seek(offset, whence);
    }

    int32_t File::Stat(FileStat *pStat) const {
        return m_pImpl->_Stat(pStat);
    }

    int32_t File::CreateDateTime(DateTime *pDate) const {
        FileStat fInfo;
        int32_t ret = m_pImpl->_Stat(&fInfo);
        if (ret >= 0) {
            *pDate = fInfo.create;
        }
        return ret;
    }

    int32_t File::ModifyDateTime(DateTime *pDate) const {
        FileStat fInfo;
        int32_t ret = m_pImpl->_Stat(&fInfo);
        if (ret >= 0) {
            *pDate = fInfo.modified;
        }
        return ret;
    }

    int32_t File::AccessDateTime(DateTime *pDate) const {
        FileStat fInfo;
        int32_t ret = m_pImpl->_Stat(&fInfo);
        if (ret >= 0) {
            *pDate = fInfo.access;
        }
        return ret;
    }

    int32_t File::Errno() const {
        return m_pImpl->_Errno();
    }
}
