#include "SockAddressImplement.h"
#include "InternalCommon.h"
#include "SockInternalUtils.h"

namespace mwl {

    static int32_t _ParseSockAddr(const sockaddr *pSockAddr, socklen_t addrLen,
        String *pHost, int32_t *pPort, SockAddressFamily *pAF) {
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
                    if (addrLen < 16) {
                        MWL_WARN("adddrLen of AF_INET should be at least 16, not %d", addrLen);
                    }
                    const sockaddr_in *pAddrIn = reinterpret_cast<const sockaddr_in *>(pSockAddr);
                    src = &pAddrIn->sin_addr;
                    port = ntohs(pAddrIn->sin_port);
                    af = SOCK_AF_INET;
                } else {
                    if (addrLen < 28) {
                        MWL_WARN("adddrLen of AF_INET6 should be at least 28, not %d", addrLen);
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
                port = 0;
                af = SOCK_AF_ABSTRACT;
#ifdef __MWL_LINUX__
                const sockaddr_un *pAddrUn = reinterpret_cast<const sockaddr_un *>(pSockAddr);
                if (addrLen < sizeof(pAddrUn->sun_family)) {
                    MWL_WARN("adddrLen of AF_UNIX should be at least %zd, not %d", sizeof(pAddrUn->sun_family), addrLen);
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
                            --pathLen;
                            strncpy(host, pAddrUn->sun_path + 1, pathLen);
                            af = SOCK_AF_ABSTRACT;
                        } else if (pathLen > 1) {
                            MWL_WARN("invalid sun_path with pathLen = %zd: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x...",
                                pathLen,
                                pAddrUn->sun_path[0], pAddrUn->sun_path[1],
                                pAddrUn->sun_path[2], pAddrUn->sun_path[3],
                                pAddrUn->sun_path[4], pAddrUn->sun_path[5]);
                        }
                        host[pathLen] = '\0';
                    }
                }
#endif
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

    SockAddress::Implement::Implement(const String &host, const String &service, SockAddressFamily af) {
        _initializeSock();
        _SetAddress(host, service, af);
    }

    SockAddress::Implement::Implement(const String &host, int32_t port, SockAddressFamily af) {
        _initializeSock();
        _SetAddress(host, port, af);
    }

    SockAddress::Implement::Implement(const sockaddr *pSockAddr, socklen_t addrLen) {
        _initializeSock();
        _SetAddress(pSockAddr, addrLen);
    }

    SockAddress::Implement::Implement(const SockAddress::Implement &src) {
        _initializeSock();
        _SetAddress(src._host.C_Str(), src._service.C_Str(), src._af);
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
        _service.Format("%d", port);
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetAddress(const String &host, int32_t port, SockAddressFamily af) {
        String service;
        service.Format("%d", port);
        return _SetAddress(host, service, af);
    }

    int32_t SockAddress::Implement::_SetAddress(const String &host, const String &service, SockAddressFamily af) {
        if (af < 0 || af >= SockAddressFamilyCount) {
            return ERR_INVAL_PARAM;
        }
        _host = host;
        _service = service;
        _af = af;
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetHost(const String &host) {
        _host = host;
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetHost(const sockaddr *pSockAddr, socklen_t addrLen) {
        _host.Clear();
        int32_t ret = _ParseSockAddr(pSockAddr, addrLen, &_host, nullptr, nullptr);
        if (ret < 0) {
            return ret;
        }
        return _Resolve();
    }

    int32_t SockAddress::Implement::_SetPort(int32_t port) {
        _service.Format("%d", port);
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
        if (!_host.Empty() && SOCK_AF_UNSPEC == _af) {
            if (_host.StartsWith("/")) {
                _af = SOCK_AF_LOCAL;
            } else if (_host.StartsWith("@")) {
                _af = SOCK_AF_ABSTRACT;
            }
        }

        _sockAddr = nullptr;
        _sockAddrLen = 0;
        int32_t ret = ERR_NONE;
        memset(&_ss, 0, sizeof(_ss));
        if (SOCK_AF_LOCAL == _af) {
#ifdef __MWL_LINUX__
            sockaddr_un *pAddrUn = reinterpret_cast<sockaddr_un *>(&_ss);
            strncpy(pAddrUn->sun_path, _host.C_Str(), sizeof(pAddrUn->sun_path) - 1);
            _sockAddrLen = SUN_LEN(pAddrUn);
            pAddrUn->sun_path[sizeof(pAddrUn->sun_path) - 1] = '\0';
            pAddrUn->sun_family = AF_UNIX;
            _sockAddr = reinterpret_cast<sockaddr *>(&_ss);
#endif
        } else if (SOCK_AF_ABSTRACT == _af) {
#ifdef __MWL_LINUX__
            sockaddr_un *pAddrUn = reinterpret_cast<sockaddr_un *>(&_ss);
            if (_host.StartsWith("@")) {
                _host = _host.LStrip("@", 1);
            }
            pAddrUn->sun_path[0] = 0;
            strncpy(pAddrUn->sun_path + 1, _host.C_Str(), sizeof(pAddrUn->sun_path) - 2);
            _sockAddrLen = SUN_LEN(pAddrUn) + 1 + strlen(pAddrUn->sun_path + 1);
            pAddrUn->sun_path[sizeof(pAddrUn->sun_path) - 1] = '\0';
            pAddrUn->sun_family = AF_UNIX;
            _sockAddr = reinterpret_cast<sockaddr *>(&_ss);
#endif
        } else if (!_host.Empty()) {
            addrinfo hint;
            memset(&hint, 0, sizeof(hint));
            hint.ai_family = s_afMap[_af];
            addrinfo *pAddrInfo = nullptr;
            ret = getaddrinfo(_host.C_Str(), _service.C_Str(), &hint, &pAddrInfo);
            if (pAddrInfo) {
                memcpy(&_ss, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
                _sockAddrLen = static_cast<int32_t>(pAddrInfo->ai_addrlen);
                freeaddrinfo(pAddrInfo);
                ret = ERR_NONE;
            } else {
                MWL_WARN("getaddrinfo for host %s and service %s failed: %s",
                    _host.C_Str(), _service.C_Str(), gai_strerror(ret));
            }
            if (ERR_NONE == ret) {
                _sockAddr = reinterpret_cast<sockaddr *>(&_ss);
            }
        }

        if (_sockAddr) {
            ret = _ParseSockAddr(_sockAddr, _sockAddrLen, &_host, &_port, &_af);
            if (ret < 0) {
                return ret;
            }
            _service.Format("%d", _port);
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
        _addr.Clear();
        _host.Clear();
        _service.Clear();
        _port = 0;
        _sockAddr = nullptr;
        _sockAddrLen = 0;
    }
}
