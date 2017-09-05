#include "inc/String.h"
#include "inc/FileSystem.h"
using namespace mwl;

#include "FileSearcher.h"

static void usage(const String &progName);

int main(int argc, const char *argv[]) {
    // 1. parse options
    // 2. start searcher
    // 3. join the search
    String progName;
    FileSystem::ParsePath(argv[0], nullptr, &progName);
    if (argc < 2) {
        usage(progName);
        return 0;
    }

    String workingDir, pattern;
    FileSearcher searcher;
    searcher.Search(workingDir, pattern);

    Array<String> results;
    searcher.GetResult(results);
    if (results.Empty()) {
        fprintf(stdout, "no file matches patter %s under %s", pattern.C_Str(), workingDir.C_Str());
    } else {
        for (int32_t i = 0; i < results.Size(); ++i) {
            fprintf(stdout, "%s/%s\n", workingDir.C_Str(), results[i].C_Str());
        }
    }

    return 0;
}

static void usage(const String &progName) {
    fprintf(stdout, "Usage:\n");
    fprintf(stdout, "\n%s [options] PATTERN\n", progName.C_Str());
    fprintf(stdout, "  search for files that match PATTERN\n");
    fprintf(stdout, "\nExample:\n");
    fprintf(stdout, "  %s *.sh\n", progName.C_Str());
    fprintf(stdout, "\nOptions:\n");
    fprintf(stdout, "  -d, --dir=name    Start searching under specified directory, default is under current directory\n");
    fprintf(stdout, "  -r, --recurse     Search for PATTERN recursively\n");
    fprintf(stdout, "  -?, --help        Show this message\n");
}
