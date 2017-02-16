#include "SockAddressImplement.h"
#include "InternalCommon.h"
#include "SockInternalUtils.h"


namespace mwl {

    static void inline _initializeSock() {
#ifdef __MWL_WIN__
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
#endif
    }

    static int32_t _ParseSockAddr(const sockaddr *pSockAddr, socklen_t /*addrLen*/, std::string *pHost, int32_t *pPort, SockAddressFamily *pAF) {
        int32_t ret = ERR_NONE;
        char host[256] = {0};
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
#ifdef __MWL_LINUX__
                    const sockaddr_un *pAddrUn = reinterpret_cast<const sockaddr_un *>(pSockAddr);
                    if (pAddrUn->sun_path[0]) {
                        strncpy(host, pAddrUn->sun_path, sizeof(host) - 1);
                        af = SOCK_AF_LOCAL;
                    } else if (pAddrUn->sun_path[1]) {
                        strncpy(host, pAddrUn->sun_path + 1, sizeof(host) - 1);
                        af = SOCK_AF_ABSTRACT;
                    }
                    host[sizeof(host) - 1] = '\0';
#endif
                    port = 0;
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
        _SetAddress(host, port, af);
    }

    SockAddress::Implement::Implement(const sockaddr *pSockAddr, socklen_t addrLen) {
        _initializeSock();
        _SetAddress(pSockAddr, addrLen);
    }

    SockAddress::Implement::Implement(const SockAddress::Implement &src) {
        _initializeSock();
        _SetAddress(src._host.c_str(), src._service.c_str(), src._af);
    }

    SockAddress::Implement::~Implement() {
    }

    int32_t SockAddress::Implement::_SetAddress(const sockaddr *pSockAddr, socklen_t addrLen) {
        _Reset();
        int32_t port = 0;
        int32_t ret = _ParseSockAddr(pSockAddr, addrLen, &_host, &port, &_af);
        _SetPort(port);
        return ret;
    }

    int32_t SockAddress::Implement::_SetAddress(const char *host, int32_t port, SockAddressFamily af) {
        char service[64] = {0};
        snprintf(service, sizeof(service), "%d", port);
        return _SetAddress(host, service, af);
    }

    int32_t SockAddress::Implement::_SetAddress(const char *host, const char *service, SockAddressFamily af) {
        _Reset();
        if (af < 0 || af >= SockAddressFamilyCount) {
            return ERR_INVAL_PARAM;
        }
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
        _host.clear();
        _sockAddr = nullptr;
        _sockAddrLen = 0;
        if (host) {
            _host = host;
        }
        return ERR_NONE;
    }

    int32_t SockAddress::Implement::_SetHost(const sockaddr *pSockAddr, socklen_t addrLen) {
        _host.clear();
        _sockAddr = nullptr;
        _sockAddrLen = 0;
        return _ParseSockAddr(pSockAddr, addrLen, &_host, nullptr, nullptr);
    }

    int32_t SockAddress::Implement::_SetPort(int32_t port) {
        char service[64];
        snprintf(service, sizeof(service), "%d", port);
        _service = service;
        _port = port;
        _sockAddr = nullptr;
        _sockAddrLen = 0;
        return ERR_NONE;
    }

    int32_t SockAddress::Implement::_SetPort(const sockaddr *pSockAddr, socklen_t addrLen) {
        int32_t port = 0;
        int32_t ret = _ParseSockAddr(pSockAddr, addrLen, nullptr, &port, nullptr);
        if (ERR_NONE == ret) {
            _SetPort(port);
        }
        _sockAddr = nullptr;
        _sockAddrLen = 0;
        return ret;
    }

    int32_t SockAddress::Implement::_SetFamily(SockAddressFamily af) {
        if (af < 0 || af >= SockAddressFamilyCount) {
            return ERR_INVAL_PARAM;
        }
        _af = af;
        _sockAddr = nullptr;
        _sockAddrLen = 0;
        return ERR_NONE;
    }

    int32_t SockAddress::Implement::_SetFamily(const sockaddr *pSockAddr, socklen_t addrLen) {
        _sockAddr = nullptr;
        _sockAddrLen = 0;
        return _ParseSockAddr(pSockAddr, addrLen, nullptr, nullptr, &_af);
    }

    int32_t SockAddress::Implement::_Resolve() {
        if (!_host.empty() && SOCK_AF_UNSPEC == _af) {
            if (_host[0] == '/') {
                _af = SOCK_AF_LOCAL;
            } else if (_host[0] == '@') {
                _af = SOCK_AF_ABSTRACT;
            }
        }

        _sockAddr = nullptr;
        _sockAddrLen = 0;
        int32_t ret = ERR_NONE;
        if (SOCK_AF_LOCAL == _af) {
#ifdef __MWL_LINUX__
            sockaddr_un *pAddrUn = reinterpret_cast<sockaddr_un *>(&_ss);
            snprintf(pAddrUn->sun_path, sizeof(pAddrUn->sun_path) - 1, "%s", _host.c_str());
            _sockAddrLen = SUN_LEN(pAddrUn);
            pAddrUn->sun_path[sizeof(pAddrUn->sun_path) - 1] = '\0';
            pAddrUn->sun_family = AF_UNIX;
            _sockAddr = reinterpret_cast<sockaddr *>(&_ss);
#endif
        } else if (SOCK_AF_ABSTRACT == _af) {
#ifdef __MWL_LINUX__
            sockaddr_un *pAddrUn = reinterpret_cast<sockaddr_un *>(&_ss);
            if (_host[0] == '@') {
                _host = _host.substr(1);
            }
            pAddrUn->sun_path[0] = 0;
            snprintf(pAddrUn->sun_path + 1, sizeof(pAddrUn->sun_path) - 2, "%s", _host.c_str());
            _sockAddrLen = SUN_LEN(pAddrUn) + 1 + strlen(pAddrUn->sun_path + 1);
            pAddrUn->sun_path[sizeof(pAddrUn->sun_path) - 1] = '\0';
            pAddrUn->sun_family = AF_UNIX;
            _sockAddr = reinterpret_cast<sockaddr *>(&_ss);
#endif
        } else {
            addrinfo hint;
            memset(&hint, 0, sizeof(hint));
            hint.ai_family = s_afMap[_af];
            addrinfo *pAddrInfo = nullptr;
            ret = getaddrinfo(_host.c_str(), _service.c_str(), &hint, &pAddrInfo);
            if (pAddrInfo) {
                memcpy(&_ss, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
                _sockAddrLen = static_cast<int32_t>(pAddrInfo->ai_addrlen);
                freeaddrinfo(pAddrInfo);
                ret = ERR_NONE;
            } else {
                MWL_WARN("getaddrinfo for host %s and service %s failed: %s",
                         _host.c_str(), _service.c_str(), gai_strerror(ret));
            }
            if (ERR_NONE == ret) {
                _sockAddr = reinterpret_cast<sockaddr *>(&_ss);
            }
        }

        if (_sockAddr) {
            _SetAddress(_sockAddr, _sockAddrLen);
        }
        return -ret;
    }

    const sockaddr *SockAddress::Implement::_SockAddr() {
        if (!_sockAddr) {
            _Resolve();
        }
        return _sockAddr;
    }

    int32_t SockAddress::Implement::_SockAddrLen() {
        return _sockAddrLen;
    }

    void SockAddress::Implement::_Reset() {
        _af = SOCK_AF_UNSPEC;
        _addr.clear();
        _host.clear();
        _service.clear();
        _port = 0;
        _sockAddr = nullptr;
        _sockAddrLen = 0;
    }
}
