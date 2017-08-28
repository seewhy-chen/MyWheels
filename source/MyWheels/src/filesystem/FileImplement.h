#include "inc/File.h"

namespace mwl {
    struct File::Implement {
        Implement();
        ~Implement();
        int32_t _Open(const String &path, uint32_t flags, uint32_t modes);
        int32_t _Close();
        int32_t _Read(ByteArray &data, int32_t len);
        int32_t _Read(void *pData, int32_t len);
        int32_t _Write(const ByteArray &data, int32_t len);
        int32_t _Write(const void *pData, int32_t len);
        bool _Opened();
        bool _Readable();
        bool _Writable();

        String _Pathname();
        String _Dirname();
        String _Basename();
        String _Filename();
        String _Extname();
        String _DotExtname();
        int64_t _Size();
        int64_t _Tell();
        int64_t _Seek(int64_t offset, FileSeekWhence whence);
        int32_t _Stat(FileStat *pStat);

    private:
        int32_t _fh;
        int32_t _flags;
        int32_t _modes;
        int32_t _err;
        String _filePath;
    };
}
