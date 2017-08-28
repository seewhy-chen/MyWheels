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
        return m_pImpl->_Open(path, FOF_WRONLY | FOF_CREATE | FOF_TRUNC, modes);
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

    const String File::Pathname() const {
        return m_pImpl->_Pathname();
    }

    const String File::Dirname() const {
        return m_pImpl->_Dirname();
    }

    const String File::Basename() const {
        return m_pImpl->_Basename();
    }

    const String File::Filename() const {
        return m_pImpl->_Filename();
    }

    const String File::Extname() const {
        return m_pImpl->_Extname();
    }

    const String File::DotExtname() const {
        return m_pImpl->_DotExtname();
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

}
