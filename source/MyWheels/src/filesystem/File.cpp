#include "inc/File.h"
#include "FileImplement.h"

namespace mwl {
    File::File() : m_pImpl(new Implement()) {}

    File::File(const String &path, uint32_t flags, uint32_t mode)
        : m_pImpl(new Implement()) {
        if (!path.Empty()) {
            m_pImpl->_Open(path, flags, mode);
        }
    }

    File::~File() {
        delete m_pImpl;
    }

    int32_t File::Open(const String &path, uint32_t flags, uint32_t mode) {
        return 0;
    }

    int32_t File::Close() {
        return 0;
    }

    int32_t File::Read(ByteArray &data, int32_t len) {
        return 0;
    }

    int32_t File::Read(void *pData, int32_t len) {
        return 0;
    }

    int32_t File::Write(const ByteArray &data, int32_t len) {
        return 0;
    }

    int32_t File::File::Write(const void *pData, int32_t len) {
        return 0;
    }

    int64_t File::Size() const {
        return 0;
    }

    int64_t File::Tell() const {
        return 0;
    }

    int64_t File::Seek(int64_t offset, FileSeekWhence whence) {
        return 0;
    }

    int32_t File::Stat(FileStat *pStat) {
        return 0;
    }

}
