#include "InternalCommon.h"
#include "inc/File.h"
using namespace mwl;

extern const char *prog;

void TestFile() {
    File f;
    MWL_ASSERT(!f.Readable());
    MWL_ASSERT(!f.Writable());

    FileStat fInfo;
    f.Open(prog);
    f.Stat(&fInfo);
    MWL_INFO("created on: %s", fInfo.create.AsLocal().ToStr().C_Str());
    MWL_INFO("modified on: %s", fInfo.modified.AsLocal().ToStr().C_Str());
    MWL_INFO("access on: %s", fInfo.access.AsLocal().ToStr().C_Str());
    MWL_ASSERT(!f.Writable());
    MWL_ASSERT(f.Readable());
    int64_t s = f.Size();
    MWL_ASSERT(s > 0);
    MWL_ASSERT(0 == f.Tell());
    f.Seek(s / 2);
    MWL_ASSERT(s/2 == f.Tell());
    f.Seek(-1, FRM_END);
    MWL_ASSERT(s - 1 == f.Tell());
    f.Seek(0);
    MWL_ASSERT(0 == f.Tell());
    ByteArray d(static_cast<int32_t>(f.Size()));
    MWL_ASSERT(s == f.Read(d, d.Size()));

    MWL_INFO("dir name: %s", f.Dirname().C_Str());
    MWL_INFO("base name: %s", f.Basename().C_Str());
    MWL_INFO("file name: %s", f.Filename().C_Str());
    MWL_INFO("ext name: %s", f.Extname().C_Str());
    MWL_INFO("dot ext name: %s", f.DotExtname().C_Str());

    f.Create("./test");
    MWL_ASSERT(f.Opened());
    MWL_ASSERT(!f.Readable());
    MWL_INFO("%s created", f.Pathname().C_Str());
    MWL_INFO("dir name: %s", f.Dirname().C_Str());
    MWL_INFO("base name: %s", f.Basename().C_Str());
    MWL_INFO("file name: %s", f.Filename().C_Str());
    MWL_INFO("ext name: %s", f.Extname().C_Str());
    MWL_INFO("dot ext name: %s", f.DotExtname().C_Str());
}