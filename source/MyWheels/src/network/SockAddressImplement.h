#ifndef __MWL_SOCK_ADDR_IMPLEMENT_H__
#define __MWL_SOCK_ADDR_IMPLEMENT_H__

#include "inc/Socket.h"

#include <WinSock2.h>
#include <string>

namespace mwl {
    
    struct SockAddress::Implement {
        Implement(const char *host, const char *service, SockAddressFamily af);
        Implement(const char *host, int32_t port, SockAddressFamily af);
        Implement(const sockaddr *pSockAddr);
        ~Implement();

        int32_t _SetAddress(const sockaddr *pSockAddr);
        int32_t _SetAddress(const char *host, const char *service, SockAddressFamily af);
        int32_t _SetHost(const char *strHost);
        int32_t _SetHost(const sockaddr *pSockAddr);
        int32_t _SetPort(int32_t port);
        int32_t _SetPort(const sockaddr *pSockAddr);
        int32_t _SetFamily(SockAddressFamily af);
        int32_t _SetFamily(const sockaddr *pSockAddr);
        const sockaddr* _RawAddr();
        
        void _Reset();
        SockAddressFamily _af;
        std::string _addr;
        std::string _host;
        std::string _service;
        sockaddr *_rawAddr;
        sockaddr_storage  _addrStorage;
    };
}

#endif // __MWL_SOCK_ADDR_IMPLEMENT_H__