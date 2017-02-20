#include "inc/Socket.h"
#include "inc/TimeDefines.h"
#include "InternalCommon.h"
using namespace mwl;

#include <string>

void TestSocket() {
    MWL_INFO("TestSocket started");

    mwl::SockAddress addr;
    addr.SetAddress("www.baidu.com", "http", mwl::SOCK_AF_INET);
    MWL_INFO("service http at 'www.baidu.com' is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("192.168.1.1", "80", mwl::SOCK_AF_INET);
    MWL_INFO("port 80 at '192.168.1.1' is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

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

    Socket s1(SOCK_AF_INET, SOCK_TYPE_DGRAM);
    int32_t reuseAddr = 1;
    s1.SetOption(SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
    if (ERR_NONE == s1.Bind("localhost")) {
        MWL_INFO("s1 is bound to %s@%d", s1.LocalAddress().Host(), s1.LocalAddress().Port());
    }
    //s1.Listen();
    //MWL_INFO("s1 is listening at %s@%d", s1.LocalAddress().Host(), s1.LocalAddress().Port());

    Socket s2(SOCK_AF_INET, SOCK_TYPE_DGRAM);
    //s2.SetOption(SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
    //if (ERR_NONE == s2.Bind("localhost")) {
    //    MWL_INFO("s2 is bound to %s@%d", s2.LocalAddress().Host(), s2.LocalAddress().Port());
    //}

    /*if (ERR_NONE == s2.Connect(s1.LocalAddress()))*/ {
        //SharedPtr<Socket> s3 = s1.Accept();
        //if (s3 != nullptr) {
        //    MWL_INFO("s2 at %s:%d is connected to %s:%d",
        //        s2.LocalAddress().Host(), s2.LocalAddress().Port(),
        //        s2.PeerAddress().Host(), s2.PeerAddress().Port());

        //    MWL_INFO("s3 at %s:%d is connected to %s:%d",
        //        s3->LocalAddress().Host(), s3->LocalAddress().Port(),
        //        s3->PeerAddress().Host(), s3->PeerAddress().Port());
        //}
        char buf[256] = {0};
        int32_t n = s2.SendAllTo("hello there", strlen("hello there") + 1, s1.LocalAddress());
        if (n > 0) {
            MWL_INFO("%d bytes has been sent to %s:%d", n, s1.LocalAddress().Host(), s1.LocalAddress().Port());
            SockAddress srcAddr;
            s1.RecvFrom(buf, 256, srcAddr);
            MWL_INFO("s1 received %s from %s:%d", buf, srcAddr.Host(), srcAddr.Port());
        }
    }
    MWL_INFO("press any key to continue...");
    getc(stdin);
    MWL_INFO("TestSocket done\n");
}
