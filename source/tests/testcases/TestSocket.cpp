#include "inc/Socket.h"
#include "inc/TimeDefines.h"
#include "inc/Thread.h"
#include "inc/Mutex.h"
#include "inc/Condition.h"
#include "InternalCommon.h"
using namespace mwl;

#include <time.h>

static void _TestSockAddress();
static void _TestTCPSocket();
static void _TestUDPSocket();

static int32_t TestSysServices(ThreadContext *);

void TestSocket() {
    MWL_INFO("TestSocket started");

    _TestSockAddress();
    _TestTCPSocket();
    _TestUDPSocket();

    SharedPtr<Thread> t = StartThread(TestSysServices);
    t->Join();

    MWL_INFO("TestSocket done, press Enter to continue...\n");
    getc(stdin);
}

void _TestSockAddress() {
    mwl::SockAddress addr;
    addr.SetAddress("www.baidu.com", "http", mwl::SOCK_AF_INET);
    MWL_INFO("service http at 'www.baidu.com' is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("1.2.3.4", mwl::SOCK_AF_INET);
    MWL_INFO("port 80 at '1.2.3.4' is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("localhost", "1234", mwl::SOCK_AF_INET);
    MWL_INFO("port 1234 at 'localhost' is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("localhost", 1234, mwl::SOCK_AF_INET6);
    MWL_INFO("IPv6 of port 1234 at 'localhost' is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("0:0:0:0:0:0:0:1", "1234", mwl::SOCK_AF_INET6);
    MWL_INFO("0:0:0:0:0:0:0:1 at port 1234 is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("/unix_sock");
    MWL_INFO("/unix_sock is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("@abstract_sock");
    MWL_INFO("@abstract_sock is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());
}


static void _TestTCPSocket() {
    Socket s1(SOCK_AF_INET, SOCK_TYPE_STREAM);
    int32_t reuseAddr = 1;
    s1.SetOption(SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
    //if (ERR_NONE == s1.Bind("localhost")) {
    //    MWL_INFO("s1 is bound to %s@%d", s1.LocalAddress().Host(), s1.LocalAddress().Port());
    //}
    s1.Listen();
    MWL_INFO("s1 is listening at %s@%d", s1.LocalAddress().Host(), s1.LocalAddress().Port());

    Socket s2(SOCK_AF_INET, SOCK_TYPE_STREAM);
    s2.SetOption(SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
    if (ERR_NONE == s2.Bind("localhost")) {
        MWL_INFO("s2 is bound to %s@%d", s2.LocalAddress().Host(), s2.LocalAddress().Port());
    }

    if (ERR_NONE == s2.Connect(s1.LocalAddress())) {
        Socket s3;
        if (ERR_NONE == s1.Accept(s3)) {
            MWL_INFO("s2 at %s:%d is connected to %s:%d",
                s2.LocalAddress().Host(), s2.LocalAddress().Port(),
                s2.PeerAddress().Host(), s2.PeerAddress().Port());

            MWL_INFO("s3 at %s:%d is connected to %s:%d",
                s3.LocalAddress().Host(), s3.LocalAddress().Port(),
                s3.PeerAddress().Host(), s3.PeerAddress().Port());
            char buf[256] = {0};
            int32_t n = s2.SendAll("hello there", strlen("hello there") + 1);
            if (n > 0) {
                MWL_INFO("%d bytes has been sent to", n);
                SockAddress srcAddr;
                s3.RecvFrom(buf, 256, srcAddr);
                MWL_INFO("s3 received %s", buf);
            }
        }
    }
}

static void _TestUDPSocket() {
    Socket s1(SOCK_AF_INET, SOCK_TYPE_DGRAM);
    int32_t reuseAddr = 1;
    s1.SetOption(SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
    if (ERR_NONE == s1.Bind("localhost")) {
        MWL_INFO("s1 is bound to %s@%d", s1.LocalAddress().Host(), s1.LocalAddress().Port());
    }

    Socket s2(SOCK_AF_INET, SOCK_TYPE_DGRAM);
    char buf[256] = {0};
    int32_t n = s2.SendAllTo("hello there", strlen("hello there") + 1, s1.LocalAddress());
    if (n > 0) {
        MWL_INFO("%d bytes has been sent to %s:%d", n, s1.LocalAddress().Host(), s1.LocalAddress().Port());
        SockAddress srcAddr;
        s1.RecvFrom(buf, 256, srcAddr);
        MWL_INFO("s1 received %s from %s:%d", buf, srcAddr.Host(), srcAddr.Port());
    }
}

static const int32_t PORT_DAYTIME = 13;
static int32_t servicePort = PORT_DAYTIME;

static int32_t MySysServer(ThreadContext *pCtx) {
    Socket svrSock(SOCK_AF_INET, SOCK_TYPE_STREAM);
    svrSock.SetOption(SOL_SOCKET, SO_REUSEADDR, 1);
    svrSock.Bind("0.0.0.0", servicePort + 3000);
    svrSock.Listen();
    MWL_INFO("my daytime server is running at: %s:%d", svrSock.LocalAddress().Host(), svrSock.LocalAddress().Port());
    TimeSpec timeout(10);
    while (!pCtx->StopQueried()) {
        Socket s;
        if (svrSock.Accept(s, &timeout) == ERR_NONE) {
            char strTime[256];
            time_t t = time(NULL);
            sprintf(strTime, "hello, I'm daytime server implemented by SeeWhy, now daytime is: %.24s", ctime(&t));
            s.SendAll(strTime, strlen(strTime) + 1);
        }
    }
    return 0;
}

static int32_t TestSysServices(ThreadContext *) {
    Socket sock(SOCK_AF_INET, SOCK_TYPE_STREAM);
#ifdef __MWL_LINUX__
    if (ERR_NONE == sock.Connect("localhost", servicePort)) {
        char buf[256] = {0};
        sock.Recv(buf, sizeof(buf));
        MWL_INFO("daytime received from 'localhost:%d' is:\n%s", servicePort, buf);
    }
#endif

    SharedPtr<Thread> server = StartThread(MySysServer);
    sock.Reopen();
    int32_t sendSize = 0;
    sock.GetOption(SOL_SOCKET, SO_SNDBUF, sendSize);
    MWL_INFO("sendSize = %d", sendSize);
    int32_t recvSize = 0;
    sock.GetOption(SOL_SOCKET, SO_RCVBUF, recvSize);
    MWL_INFO("recvSize = %d", recvSize);
    if (ERR_NONE == sock.Connect("localhost", servicePort + 3000)) {
        char buf[256*1024] = {0};
        sock.Recv(buf, sizeof(buf));
        MWL_INFO("daytime received from 'localhost:%d' is:\n%s", servicePort + 3000, buf);
    }
    return 0;
}