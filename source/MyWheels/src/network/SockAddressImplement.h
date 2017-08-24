#ifndef __MWL_SOCK_ADDR_IMPLEMENT_H__
#define __MWL_SOCK_ADDR_IMPLEMENT_H__

#include "inc/Socket.h"
#include "SockInternalUtils.h"

namespace mwl {

    struct SockAddress::Implement {
        Implement(const String &host, const String &service, SockAddressFamily af);
        Implement(const String &host, int32_t port, SockAddressFamily af);
        Implement(const sockaddr *pSockAddr, socklen_t addrLen);
        Implement(const Implement &src);
        ~Implement();

        int32_t _SetAddress(const String &host, int32_t port, SockAddressFamily af);
        int32_t _SetAddress(const String &host, const String &service, SockAddressFamily af);
        int32_t _SetAddress(const sockaddr *pSockAddr, socklen_t addrLen);

        int32_t _SetHost(const String &strHost);
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
        String _addr;
        String _host;
        String _service;
        int32_t _port;
        sockaddr *_sockAddr;
        int32_t _sockAddrLen;
        sockaddr_storage _ss;
    };
}

#endif // __MWL_SOCK_ADDR_IMPLEMENT_H__
