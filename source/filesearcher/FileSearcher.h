#ifndef __FILE_SEARCHER_H__
#define __FILE_SEARCHER_H__

#include "inc/BasicDefines.h"
#include "inc/Array.h"
#include "inc/String.h"
using namespace mwl;

class FileSearcher {
public:
    FileSearcher();
    int32_t Search(const String &workingDir, const String &pattern);
    int32_t GetResult(Array<String> &results);
};

#endif
