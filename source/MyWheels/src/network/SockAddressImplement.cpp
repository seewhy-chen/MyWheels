#include "SockAddressImplement.h"
#include "InternalCommon.h"

#ifdef __MWL_WIN__
    #include <ws2ipdef.h> // for sockaddr_in6
    #include <WS2tcpip.h> // for inet_ntop
#elif defined __MWL_LINUX__
#endif

namespace mwl {

    static void inline _initializeSock() {
#define WS_VERSION_CHOICE1 0x202    /*MAKEWORD(2,2)*/
#define WS_VERSION_CHOICE2 0x101    /*MAKEWORD(1,1)*/
        static bool _winSockInitialized = false;
        WSADATA wsadata;
        if (!_winSockInitialized) {
            int32_t ret = WSAStartup(WS_VERSION_CHOICE1, &wsadata);
            if (ret != 0) {
                MWL_WARN("WSAStartup failed, ret = %d", ret);
                ret = WSAStartup(WS_VERSION_CHOICE2, &wsadata);
                if (ret != 0) {
                    MWL_WARN("WSAStartup2 failed, ret = %d", ret);
                }
                return;
            }
            if ((wsadata.wVersion != WS_VERSION_CHOICE1)
                    && (wsadata.wVersion != WS_VERSION_CHOICE2)) {
                WSACleanup();
                MWL_WARN("invalid winsock version: 0x%x(%d)", wsadata.wVersion, wsadata.wVersion);
            } else {
                _winSockInitialized = true;
            }
        }
    }

    static int32_t s_afMap[] = {
        AF_UNSPEC,  // SOCK_AF_UNSPEC
        AF_INET,    // SOCK_AF_INET
        AF_INET6,   // SOCK_AF_INET6
        AF_UNIX,    // SOCK_AF_LOCAL

    };
    MWL_STATIC_ASSERT(MWL_ARR_SIZE(s_afMap) == SockAddressFamilyCount, some_sock_family_is_missing);

    static int32_t _ParseSockAddr(const sockaddr *pSockAddr, std::string *pHost, int32_t *pPort, SockAddressFamily *pAF) {
        int32_t ret = ERR_NONE;
        char host[512] = {0};
        uint16_t port = 0;
        SockAddressFamily af = SOCK_AF_INVALID;
        if (pSockAddr) {
            switch (pSockAddr->sa_family) {
            case AF_INET:
            case AF_INET6: {
                    const void *src = nullptr;
                    if (AF_INET == pSockAddr->sa_family) {
                        const sockaddr_in *pAddrIn = reinterpret_cast<const sockaddr_in *>(pSockAddr);
                        src = &pAddrIn->sin_addr;
                        port = ntohs(pAddrIn->sin_port);
                        af = SOCK_AF_INET;
                    } else {
                        const sockaddr_in6 *pAddrIn6 = reinterpret_cast<const sockaddr_in6 *>(pSockAddr);
                        src = &pAddrIn6->sin6_addr;
                        port = ntohs(pAddrIn6->sin6_port);
                        af = SOCK_AF_INET6;
                    }
                    if (!inet_ntop(pSockAddr->sa_family, const_cast<void *>(src), host, sizeof(host))) {
                        int32_t err = errno;
                        MWL_WARN_ERRNO("inet_ntop failed", err);
                        ret = -err;
                    }
                }
                break;
            case AF_UNIX: {
                    af = SOCK_AF_LOCAL;
                }
                break;
            default: {
                    MWL_WARN("un-support sa_family: 0x%x(%d)", pSockAddr->sa_family, pSockAddr->sa_family);
                    ret = ERR_INVAL_PARAM;
                }
                break;
            }
        }

        if (pHost) {
            *pHost = host;
        }
        if (pPort) {
            *pPort = port;
        }
        if (pAF) {
            *pAF = af;
        }
        return ret;
    }

    SockAddress::Implement::Implement(const char *host, const char *service, SockAddressFamily af) {
        _initializeSock();
        _SetAddress(host, service, af);
    }

    SockAddress::Implement::Implement(const char *host, int32_t port, SockAddressFamily af) {
        _initializeSock();
        char service[64] = {0};
        snprintf(service, sizeof(service), "%d", port);
        _SetAddress(host, service, af);
    }

    SockAddress::Implement::Implement(const sockaddr *pSockAddr) {
        _initializeSock();
        _SetAddress(pSockAddr);
    }

    SockAddress::Implement::~Implement() {
    }

    int32_t SockAddress::Implement::_SetAddress(const sockaddr *pSockAddr) {
        _Reset();
        int32_t port = 0;
        int32_t ret = _ParseSockAddr(pSockAddr, &_host, &port, &_af);
        if (ERR_NONE == ret) {
            _SetPort(port);
        }
        return ret;
    }

    int32_t SockAddress::Implement::_SetAddress(const char *host, const char *service, SockAddressFamily af) {
        if (af < 0 || af >= SockAddressFamilyCount) {
            return ERR_INVAL_PARAM;
        }
        _Reset();
        if (host) {
            _host = host;
        }
        if (service) {
            _service = service;
        }
        _af = af;
        return ERR_NONE;
    }

    int32_t SockAddress::Implement::_SetHost(const char *host) {
        if (host) {
            _host = host;
        }
        else {
            _host.clear();
        }
        return ERR_NONE;
    }

    int32_t SockAddress::Implement::_SetHost(const sockaddr *pSockAddr) {
        return _ParseSockAddr(pSockAddr, &_host, NULL, NULL);
    }

    int32_t SockAddress::Implement::_SetPort(int32_t port) {
        char service[64];
        snprintf(service, sizeof(service), "%d", port);
        _service = service;
        _port = port;
        return ERR_NONE;
    }

    int32_t SockAddress::Implement::_SetPort(const sockaddr *pSockAddr) {
        int32_t port = 0;
        int32_t ret = _ParseSockAddr(pSockAddr, NULL, &port, NULL);
        if (ERR_NONE == ret) {
            _SetPort(port);
        }
        return ret;
    }

    int32_t SockAddress::Implement::_SetFamily(SockAddressFamily af) {
        if (af < 0 || af >= SockAddressFamilyCount) {
            return ERR_INVAL_PARAM;
        }
        _af = af;
        return ERR_NONE;
    }

    int32_t SockAddress::Implement::_SetFamily(const sockaddr *pSockAddr) {
        return _ParseSockAddr(pSockAddr, NULL, NULL, &_af);
    }

    int32_t SockAddress::Implement::_Resolve() {
        addrinfo hint;
        memset(&hint, 0, sizeof(hint));
        hint.ai_family = s_afMap[_af];
        addrinfo *pAddrInfo = NULL;
        int32_t ret = getaddrinfo(_host.c_str(), _service.c_str(), &hint, &pAddrInfo);
        if (pAddrInfo) {
            memcpy(&_ss, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
            _rawAddr = reinterpret_cast<sockaddr*>(&_ss);
            freeaddrinfo(pAddrInfo);
        }
        else {
            MWL_WARN("getaddrinfo for host %s and service %s failed: %s",
                     _host.c_str(), _service.c_str(), gai_strerror(ret));
        }
        if (ERR_NONE == ret) {
            _rawAddr = reinterpret_cast<sockaddr*>(&_ss);
        }
        return ret;
    }

    const sockaddr* SockAddress::Implement::_RawAddr() {
        if (!_rawAddr) {
            _Resolve();
        }
        return _rawAddr;
    }

    void SockAddress::Implement::_Reset() {
        _af = SOCK_AF_UNSPEC;
        _addr.clear();
        _host.clear();
        _service.clear();
        _port = 0;
        _rawAddr = nullptr;
    }
}
