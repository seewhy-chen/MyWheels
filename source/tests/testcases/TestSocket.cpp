#include "inc/Socket.h"
#include "InternalCommon.h"
using namespace mwl;

#include <string>

void TestSocket() {
    MWL_INFO("TestSocket started");

    mwl::SockAddress addr;
    addr.SetAddress("www.baidu.com", "http", mwl::SOCK_AF_INET);
    addr.Resolve();
    MWL_INFO("service http at www.baidu.com is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("0:0:0:0:0:0:0:1", "1234", mwl::SOCK_AF_INET6);
    addr.Resolve();
    MWL_INFO("0:0:0:0:0:0:0:1 at port 1234 is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("/unix_sock");
    addr.Resolve();
    MWL_INFO("/unix_sock is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    addr.SetAddress("@abstract_sock");
    addr.Resolve();
    MWL_INFO("@abstract_sock is resolved as: %s@%d in family %d", addr.Host(), addr.Port(), addr.Family());

    MWL_INFO("TestSocket done\n");
}
