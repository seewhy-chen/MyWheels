#include "FileSearcher.h"
#include "inc/Thread.h"
#include "inc/String.h"
#include "inc/FileSystem.h"
#include "inc/Mutex.h"
#include "inc/Semaphore.h"
using namespace mwl;

struct FileSpiderShareData {
    Array<String> *pDirsToSearch;
    Semaphore *pSem;
    Mutex *pMutex;
    String workingDir;
    String pattern;
    Mutex lock;
    Array<String> results;
    bool recursive;
};

MWL_THREAD_ENTRY(_FileSpider, pContext) {
    FileSpiderShareData *pSharedData = pContext->SharedData<FileSpiderShareData>();
    Array<String> subDirs, files;
    FileSystem::ListDir(pSharedData->workingDir, &subDirs, &files);
    for (int32_t i = 0; i < files.Size(); ++i) {
        // TODO: check if files[i] matches pattern
    }
    if (pSharedData->recursive) {
        for (int32_t i = 0; i < subDirs.Size(); ++i) {
            // TODO: search in 'pSharedData->workingDir + "/" + subDirs[i]' for pattern
        }
    }
    return 0;
}

FileSearcher::FileSearcher() {
}

int32_t FileSearcher::Search(const String &workingDir, const String &/*pattern*/) {
    Array<Thread*> spiders;
    Array<String> dirsToSearch;
    Semaphore sem;
    Mutex mutex;
    sem.Open("dirs", 0);
    FileSpiderShareData sharedData;
    sharedData.pDirsToSearch = &dirsToSearch;
    sharedData.pSem = &sem;
    sharedData.pMutex = &mutex;
    sharedData.workingDir = workingDir;
    spiders.Append(new Thread());
    spiders.Back()->Start(_FileSpider, &sharedData);
    while (sem.Wait() >= 0) {
        if (dirsToSearch.Empty()) {
            break;
        }
        mutex.Lock();
        TimeSpan ts(0);
        for (int32_t i = 0; i < dirsToSearch.Size(); ++i) {
            bool dispatched = false;
            for (int32_t j = 0; j < spiders.Size() && !dispatched; ++j) {
                if (spiders[j]->Join(ts) >= 0) {
                    // dispatch dirsToSearch[i] to spider[j]
                    dispatched = true;
                }
            }
            if (!dispatched) {
                spiders.Append(new Thread());
                // dispatch dirsToSearch[i] to spiders.Back()
            }
        }
        dirsToSearch.Clear();
        mutex.Unlock();
    }

    for (int32_t i = 0; i < spiders.Size(); ++i) {
        spiders[i]->Join();
        delete spiders[i];
    }
    return 0;
}

int32_t FileSearcher::GetResult(Array<String> &/*results*/) {
    return 0;
}
