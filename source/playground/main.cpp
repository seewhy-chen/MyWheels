#include <stdio.h>
#include <string>

#include "inc/Semaphore.h"
#include "inc/Mutex.h"
#include "inc/Thread.h"
#include "inc/TimeDefines.h"
#include "inc/Randomizer.h"
using namespace mwl;

struct Rendezvous {
    Semaphore s1;
    Semaphore s2;
};

int32_t EntryA(ThreadContext* pCtx) {
    Rendezvous *pRend = reinterpret_cast<Rendezvous*>(pCtx->SharedData());
    TimeSleep(1000);
    printf("statement a1\n");
    pRend->s1.Post();
    pRend->s2.Wait();
    printf("statement a2\n");

    return 0;
}

int32_t EntryB(ThreadContext* pCtx) {
    Rendezvous *pRend = reinterpret_cast<Rendezvous*>(pCtx->SharedData());
    TimeSleep(1000);
    printf("statement b1\n");
    pRend->s2.Post();
    pRend->s1.Wait();
    printf("statement b2\n");

    return 0;
}

struct SemMutex {
    Semaphore s;
    SemMutex() { s.Open("Mutex", 1); }
    int32_t Lock() {
        return s.Wait();
    }

    int32_t Unlock() {
        return s.Post();
    }
};

static int32_t cnt = 0;
int32_t EntryCnt(ThreadContext *pCtx) {

    SemMutex *pMutex = reinterpret_cast<SemMutex*>(pCtx->SharedData());
    for (int32_t i = 0; i < 100; ++i) {
        pMutex->Lock();
        int32_t tmp = cnt;
        TimeSleep(1);
        cnt = tmp + 1;
        pMutex->Unlock();
    }
    return 0;
}

struct Barrior {
    int32_t n;
    int32_t arrived;
    Semaphore s1;
    Semaphore s2;
    int32_t v2;
    Mutex l;

    Barrior(int32_t N) : n(N) {
        s1.Open("Phase1", 0);
        s2.Open("Phase2", 0);
        arrived = 0;
    }

    int32_t Arrived() {
        l.Lock();
        arrived += 1;
        if (arrived == n) {
            s1.Post(n);
        }
        l.Unlock();

        s1.Wait();
        return 0;
    }

    int32_t Passed() {
        l.Lock();
        arrived -= 1;
        if (arrived == 0) {
            s2.Post(n);
        }
        l.Unlock();

        s2.Wait();
        return 0;
    }
};

Randomizer _rand;
Mutex _m;
int32_t EntryBarrior(ThreadContext *pCtx) {
    Barrior *pBarrior = reinterpret_cast<Barrior*>(pCtx->SharedData());

    for (int32_t i = 0; i < 5; ++i) {
        MWL_INFO("%s arrived and waiting...", pCtx->Tag());
        pBarrior->Arrived();

        _m.Lock();
        int32_t sleepTime = _rand.NextS32InRange(10, 501);
        _m.Unlock();
        MWL_INFO("%s waiting done, sleep %d ms", pCtx->Tag(), sleepTime);
        TimeSleep(sleepTime);

        pBarrior->Passed();
        MWL_INFO("%s passed", pCtx->Tag());
    }
    return 0;
}

int32_t main() {
    MWL_INFO("started...");
    //Rendezvous r;
    //r.s1.Open("a1IsDone", 0);
    //r.s2.Open("b1IsDone", 0);
    //for (int32_t i = 0; i < 10; ++i) {
    //    Thread a("A");
    //    Thread b("B");
    //    a.Start(EntryA, &r);
    //    b.Start(EntryB, &r);
    //    printf("\n");
    //}

    //SemMutex m;
    //Thread a;
    //Thread b;
    //a.Start(EntryCnt, &m);
    //b.Start(EntryCnt, &m);

    //a.Join();
    //b.Join();
    //printf("cnt = %d\n", cnt);

    const int32_t N = 10;
    Barrior b(N);
    Thread threads[N];
    for (int32_t i = 0; i < N; ++i) {
        char tag[32] = { 0 };
        sprintf(tag, "%03d", i);
        threads[i].SetTag(tag);
        threads[i].Start(EntryBarrior, &b);
    }
    for (int32_t i = 0; i < N; ++i) {
        threads[i].Join();
    }


    printf("\npress any key to continue...\n");
    getc(stdin);
    return 0;
}