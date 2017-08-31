#include "InternalCommon.h"
#include "inc/File.h"
#include "inc/FileSystem.h"
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

    MWL_INFO("dir name: %s", f.DirName().C_Str());
    MWL_INFO("base name: %s", f.BaseName().C_Str());
    MWL_INFO("file name: %s", f.FileName().C_Str());
    MWL_INFO("ext name: %s", f.ExtName().C_Str());

    f.Create("./test.txt");
    MWL_ASSERT(f.Opened());
    MWL_ASSERT(!f.Readable());
    MWL_ASSERT(f.Writable());
    String content = "this is a test\n"; 
    MWL_INFO("%s created", f.PathName().C_Str());
    MWL_ASSERT(f.Write(content.C_Str(), content.Len()) == content.Len());
    MWL_INFO("dir name: %s", f.DirName().C_Str());
    MWL_INFO("base name: %s", f.BaseName().C_Str());
    MWL_INFO("file name: %s", f.FileName().C_Str());
    MWL_INFO("ext name: %s", f.ExtName().C_Str());
    f.Close();
    MWL_ASSERT(content.Len() == FileSystem::DuplicateFile("./test.txt", "./test_copied.txt"));
    MWL_ASSERT(FileSystem::IsFile("./test_copied.txt"));
    FileSystem::RemoveFile("./test.txt");
    FileSystem::RemoveFile("./test_copied.txt");
    MWL_ASSERT(!FileSystem::Exist("./test.txt"));
    MWL_ASSERT(!FileSystem::Exist("./test_copied.txt"));

    String relativePath(".");
    MWL_INFO("%s is resolved to %s", relativePath.C_Str(), FileSystem::GetFullPath(relativePath).C_Str());
    relativePath = "..";
    MWL_INFO("%s is resolved to %s", relativePath.C_Str(), FileSystem::GetFullPath(relativePath).C_Str());

    String dirName, fileName, ext;
    FileSystem::ParsePath(FileSystem::GetFullPath(relativePath) + "/.test.txt", &dirName, &fileName, &ext);
    MWL_INFO("dirName: %s, fileName: %s, ext: %s", dirName.C_Str(), fileName.C_Str(), ext.C_Str());
    MWL_INFO("made a path: %s", FileSystem::MakePath(dirName, fileName, ext).C_Str());

    Array<String> subDirs, files;
    dirName = "..";
    MWL_INFO("listing %s", dirName.C_Str());
    FileSystem::ListDir(dirName, &files, &subDirs);
    for (int32_t i = 0; i < subDirs.Size(); ++i) {
        MWL_INFO("subdir: %s", subDirs[i].C_Str());
    }
    for (int32_t i = 0; i < files.Size(); ++i) {
        MWL_INFO("file: %s", files[i].C_Str());
    }

    FileSystem::MakeDir("./new_dir");
    MWL_ASSERT(FileSystem::IsDir("./new_dir"));
    FileSystem::Move("./new_dir", "./new_dir_moved");
    MWL_ASSERT(!FileSystem::Exist("./new_dir"));
    MWL_ASSERT(FileSystem::IsDir("./new_dir_moved"));

    FileSystem::RemoveDir("./new_dir_moved");
    MWL_ASSERT(!FileSystem::Exist("./new_dir_moved"));
}