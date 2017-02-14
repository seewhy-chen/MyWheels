#include "inc/BasicDefines.h"
#include "inc/Socket.h"

#include <WinSock2.h>
#include <ws2ipdef.h>

#include <stdlib.h>
#include "inc/Socket.h"

extern void TestBasicTypes();
extern void TestTimeCounter();
extern void TestThread();
extern void TestMutex();
extern void TestCondition();
extern void TestSemaphore();
extern void TestByteArray();

int main(int, char *[]) {
    sockaddr_in addr4;
    memset(&addr4, 0, sizeof(addr4));
    addr4.sin_family = AF_INET;
    mwl::SockAddress addr(reinterpret_cast<sockaddr*>(&addr4));

    sockaddr_in6 addr6;
    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = 1234;
    addr.SetAddress(reinterpret_cast<sockaddr*>(&addr6));
    addr.SetAddress("www.baidu.com", "http", mwl::SOCK_AF_INET);
    addr.SetAddress("0:0:0:0:0:0:0:1", -1, mwl::SOCK_AF_INET6);
    TestBasicTypes();
    TestTimeCounter();
    TestThread();
    TestMutex();
    TestCondition();
    TestSemaphore();
    TestByteArray();

#ifdef __MWL_WIN__
    system("pause");
#endif
    return 0;
}
