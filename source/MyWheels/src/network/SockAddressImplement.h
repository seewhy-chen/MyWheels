#ifndef __MWL_SOCK_ADDR_IMPLEMENT_H__
#define __MWL_SOCK_ADDR_IMPLEMENT_H__

#include "inc/Socket.h"

#ifdef __MWL_WIN__
    #include <WinSock2.h>
#endif

#include <string>

namespace mwl {

    struct SockAddress::Implement {
        Implement(const char *host, const char *service, SockAddressFamily af);
        Implement(const char *host, int32_t port, SockAddressFamily af);
        Implement(const sockaddr *pSockAddr, socklen_t addrLen);
        Implement(const Implement &src);
        ~Implement();

        int32_t _SetAddress(const char *host, int32_t port, SockAddressFamily af);
        int32_t _SetAddress(const char *host, const char *service, SockAddressFamily af);
        int32_t _SetAddress(const sockaddr *pSockAddr, socklen_t addrLen);

        int32_t _SetHost(const char *strHost);
        int32_t _SetHost(const sockaddr *pSockAddr, socklen_t addrLen);
        int32_t _SetPort(int32_t port);
        int32_t _SetPort(const sockaddr *pSockAddr, socklen_t addrLen);
        int32_t _SetFamily(SockAddressFamily af);
        int32_t _SetFamily(const sockaddr *pSockAddr, socklen_t addrLen);
        int32_t _Resolve();
        const sockaddr *_SockAddr();
        int32_t _SockAddrLen();

        void _Reset();
        SockAddressFamily _af;
        std::string _addr;
        std::string _host;
        std::string _service;
        int32_t _port;
        sockaddr *_sockAddr;
        int32_t _sockAddrLen;
        sockaddr_storage _ss;
    };
}

#endif // __MWL_SOCK_ADDR_IMPLEMENT_H__