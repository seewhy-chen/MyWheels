#include "SockAddressImplement.h"
#include "InternalCommon.h"
#include "SockInternalUtils.h"

namespace mwl {

    static int32_t _ParseSockAddr(const sockaddr *pSockAddr, socklen_t addrLen,
                                  std::string *pHost, int32_t *pPort, SockAddressFamily *pAF) {
        int32_t ret = ERR_NONE;
        char host[256] = { 0 };
        uint16_t port = 0;
        SockAddressFamily af = SOCK_AF_INVALID;
        if (pSockAddr) {
            switch (pSockAddr->sa_family) {
                case AF_INET:
                case AF_INET6: {
                    const void *src = nullptr;
                    if (AF_INET == pSockAddr->sa_family) {
                        if (addrLen != 16) {
                            MWL_WARN("adddrLen of AF_INET should be 16, not %d", addrLen);
                        }
                        const sockaddr_in *pAddrIn = reinterpret_cast<const sockaddr_in *>(pSockAddr);
                        src = &pAddrIn->sin_addr;
                        port = ntohs(pAddrIn->sin_port);
                        af = SOCK_AF_INET;
                    } else {
                        if (addrLen != 28) {
                            MWL_WARN("adddrLen of AF_INET6 should be 16, not %d", addrLen);
                        }
                        const sockaddr_in6 *pAddrIn6 = reinterpret_cast<const sockaddr_in6 *>(pSockAddr);
                        src = &pAddrIn6->sin6_addr;
                        port = ntohs(pAddrIn6->sin6_port);
                        af = SOCK_AF_INET6;
                    }
                    if (!inet_ntop(pSockAddr->sa_family, const_cast<void *>(src), host, sizeof(host))) {
                        ret = -sock_errno;
                        MWL_WARN_ERRNO("inet_ntop failed", -ret);
                    }
                }
                break;
                case AF_UNIX: {
    #ifdef __MWL_LINUX__
                    const sockaddr_un *pAddrUn = reinterpret_cast<const sockaddr_un *>(pSockAddr);
                    if (addrLen < sizeof(pAddrUn->sun_family)) {
                        MWL_WARN("adddrLen of AF_UNIX should be at least 2, not %d", addrLen);
                    } else {
                        size_t pathLen = addrLen - sizeof(pAddrUn->sun_family);
                        if (pathLen > sizeof(host) - 1) {
                            pathLen = sizeof(host) - 1;
                        }
                        if (pathLen > 0) {
                            if (pAddrUn->sun_path[0]) {
                                strncpy(host, pAddrUn->sun_path, pathLen);
                                af = SOCK_AF_LOCAL;
                            } else if (pAddrUn->sun_path[1]) {
                                strncpy(host, pAddrUn->sun_path + 1, pathLen);
                                af = SOCK_AF_ABSTRACT;
                            } else {
                                MWL_WARN("invalid sun_path: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x...",
                                         pAddrUn->sun_path[0], pAddrUn->sun_path[1],
                                         pAddrUn->sun_path[2], pAddrUn->sun_path[3],
                                         pAddrUn->sun_path[4], pAddrUn->sun_path[5]);
                            }
                            host[pathLen] = '\0';
                            MWL_INFO("host = %s, addrLen = %d", host, addrLen);
                        }
                    }
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

    SockAddress::Implement::~Implement() {}

    int32_t SockAddress::Implement::_SetAddress(const sockaddr *pSockAddr, socklen_t addrLen) {
        if (!pSockAddr || addrLen <= 0) {
            _Reset();
        }
        int32_t port = 0;
        int32_t ret = _ParseSockAddr(pSockAddr, addrLen, &_host, &port, &_af);
        if (ret < 0) {
            return ret;
        }
        char service[64];
        snprintf(service, sizeof(service), "%d", port);
        _service = service;
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetAddress(const char *host, int32_t port, SockAddressFamily af) {
        char service[64] = { 0 };
        snprintf(service, sizeof(service), "%d", port);
        return _SetAddress(host, service, af);
    }

    int32_t SockAddress::Implement::_SetAddress(const char *host, const char *service, SockAddressFamily af) {
        if (af < 0 || af >= SockAddressFamilyCount) {
            return ERR_INVAL_PARAM;
        }
        _host.clear();
        if (host) {
            _host = host;
        }
        _service.clear();
        if (service) {
            _service = service;
        }
        _af = af;
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetHost(const char *host) {
        _host.clear();
        if (host) {
            _host = host;
        }
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetHost(const sockaddr *pSockAddr, socklen_t addrLen) {
        _host.clear();
        int32_t ret = _ParseSockAddr(pSockAddr, addrLen, &_host, nullptr, nullptr);
        if (ret < 0) {
            return ret;
        }
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetPort(int32_t port) {
        char service[64];
        snprintf(service, sizeof(service), "%d", port);
        _service = service;
        _port = port;
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetPort(const sockaddr *pSockAddr, socklen_t addrLen) {
        int32_t port = 0;
        int32_t ret = _ParseSockAddr(pSockAddr, addrLen, nullptr, &port, nullptr);
        if (ret < 0) {
            return ret;
        }
        return _SetPort(port);
    }

    int32_t SockAddress::Implement::_SetFamily(SockAddressFamily af) {
        if (af < 0 || af >= SockAddressFamilyCount) {
            return ERR_INVAL_PARAM;
        }
        _af = af;
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetFamily(const sockaddr *pSockAddr, socklen_t addrLen) {
        int32_t ret = _ParseSockAddr(pSockAddr, addrLen, nullptr, nullptr, &_af);
        if (ret < 0) {
            return ret;
        }
        return _Resolve();
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
            MWL_INFO("SUN_LEN(pAddrUn) = %zd, strlen(pAddrUn->sun_path + 1) = %zd", SUN_LEN(pAddrUn), strlen(pAddrUn->sun_path + 1));
            _sockAddrLen = SUN_LEN(pAddrUn) + 0 + strlen(pAddrUn->sun_path + 1);
            MWL_INFO("_sockAddrLen = %d", _sockAddrLen);
            pAddrUn->sun_path[sizeof(pAddrUn->sun_path) - 1] = '\0';
            pAddrUn->sun_family = AF_UNIX;
            _sockAddr = reinterpret_cast<sockaddr *>(&_ss);
#endif
        } else if (!_host.empty()) {
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
            int32_t port = 0;
            ret = _ParseSockAddr(_sockAddr, _sockAddrLen, &_host, &_port, &_af);
            if (ret < 0) {
                return ret;
            }
            char service[64];
            snprintf(service, sizeof(service), "%d", port);
            _service = service;
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
        if (!_sockAddr) {
            _Resolve();
        }
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
