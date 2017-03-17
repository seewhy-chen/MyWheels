#include "SocketImplement.h"
#include "SockInternalUtils.h"

namespace mwl {

    Socket::Implement::Implement() {
        _initializeSock();
        _Reset();
    }

    Socket::Implement::~Implement() {
        _Close();
    }

    int32_t Socket::Implement::_Open(int32_t af, int32_t type, int32_t protocol) {
        if (af < 0 || af >= SockAddressFamilyCount) {
            MWL_WARN("invalid af: %d", af);
            return ERR_INVAL_PARAM;
        }
        if (type < 0 || type >= SockTypeCount) {
            MWL_WARN("invalid type: %d", type);
            return ERR_INVAL_PARAM;
        }
        if (protocol < 0 || protocol >= SockProtocolCount) {
            MWL_WARN("invalid protocol: %d", protocol);
            return ERR_INVAL_PARAM;
        }

        int32_t ret = _Close();
        if (ret == ERR_NONE) {
            _sock = socket(s_afMap[af], s_typeMap[type], s_protoMap[protocol]);
            if (INVALID_SOCKET == _sock) {
                ret = -sock_errno;
                MWL_ERR_ERRNO("open socket(%d, %d, %d) failed", -ret, af, type, protocol);
            } else {
                _af = static_cast<SockAddressFamily>(af);
                _type = static_cast<SockType>(type);
                _proto = static_cast<SockProtocol>(protocol);
            }
        }
        return ret;
    }

    int32_t Socket::Implement::_Reopen() {
        if (INVALID_SOCKET == _sock) {
            return -EBADF;
        }
        SockAddressFamily af = _af;
        SockType type = _type;
        SockProtocol proto = _proto;
        int32_t ret = _Close();
        if (ERR_NONE == ret) {
            ret = _Open(af, type, proto);
        }
        return ret;
    }

    int32_t Socket::Implement::_Shutdown(int32_t how) {
        if (how < 0 || how >= SockShutdownCount) {
            MWL_WARN("invalid how: %d", how);
            return ERR_INVAL_PARAM;
        }

        int32_t ret = ERR_NONE;
        if (shutdown(_sock, s_shutdownMap[how]) < 0) {
            ret = -sock_errno;
            MWL_ERR_ERRNO("shutdown failed", -ret);
        }
        return ret;
    }

    int32_t Socket::Implement::_Close() {
        int32_t ret = ERR_NONE;
        if (_sock != INVALID_SOCKET && _sock >= 0) {
            ret = closesocket(_sock);
            if (ret < 0) {
                ret = -sock_errno;
                MWL_ERR_ERRNO("close sock failed", -ret);
            } else {
#ifdef __MWL_LINUX__
                if (_af == SOCK_AF_LOCAL) {
                    unlink(_localAddr.Host());
                }
#endif
            }
        }
        _Reset();
        return ret;
    }

    int32_t Socket::Implement::_SetHandle(SockHandle handle, SockAddressFamily af, SockType type, SockProtocol protocol) {
        _Close();
        if (SOCK_TYPE_INVALID == type) {
            int32_t sockType = -1;
            socklen_t optLen = sizeof(sockType);
            int32_t ret = _GetOption(SOL_SOCKET, SO_TYPE, &sockType, &optLen);
            if (ret < 0) {
                return ret;
            }
            switch (sockType) {
            case SOCK_STREAM:
                type = SOCK_TYPE_STREAM;
                break;
            case SOCK_DGRAM:
                type = SOCK_TYPE_DGRAM;
                break;
            case SOCK_SEQPACKET:
                type = SOCK_TYPE_SEQPKT;
                break;
            default: {
                    MWL_WARN("un-support sock type %d", sockType);
                    return ERR_INVAL_PARAM;
                }
                break;
            }
        }
        _sock = handle;
        _af = af;
        _type = type;
        _proto = protocol;

#ifdef _WIN32
        _SetNonblocking(false);
#else
        int32_t sockFlags = fcntl(_sock, F_GETFL, 0);
        if (sockFlags >= 0) {
            _nonblocking = (sockFlags & O_NONBLOCK);
        } else {
            _SetNonblocking(false);
        }
#endif
        _UpdateLocalAddr();
        _UpdatePeerAddr();
        return ERR_NONE;
    }

    SockHandle Socket::Implement::_Handle() {
        return _sock;
    }

    void Socket::Implement::_Reset() {
        _sock = INVALID_SOCKET;
        _af = SOCK_AF_INVALID;
        _type = SOCK_TYPE_INVALID;
        _proto = SOCK_PROTO_INVALID;
        _nonblocking = false;
        _blkStB4Listening = false;
        _localAddr.Reset();
        _peerAddr.Reset();
    }

    int32_t Socket::Implement::_Bind(const SockAddress &address) {
        int32_t ret = ERR_NONE;
        if (bind(_sock, address.SockAddr(), address.SockAddrLen()) < 0) {
            ret = -sock_errno;
            MWL_ERR_ERRNO("bind to %s at port %d as af %s failed", -ret, address.Host(), address.Port(), address.FamilyName());
            ret = -sock_errno;
        } else {
            _UpdateLocalAddr();
        }
        return ret;
    }

    int32_t Socket::Implement::_Listen(int32_t backlog) {
        int32_t ret = ERR_NONE;
        _blkStB4Listening = _nonblocking;
        _SetNonblocking(true); // refer to UNP Chapter 16.6: Nonblocking accept
        if (listen(_sock, backlog) < 0) {
            ret = -sock_errno;
            MWL_ERR_ERRNO("listen on %s at port %d as af %s failed",
                          -ret, _localAddr.Host(), _localAddr.Port(), _localAddr.FamilyName());
        } else {
            _UpdateLocalAddr();
        }
        return ret;
    }

    int32_t Socket::Implement::_Connect(const SockAddress &address, const TimeSpec *pTimeout) {
        bool origNonblocking = _nonblocking;
        int32_t ret = ERR_NONE;
        if (pTimeout) {
            _SetNonblocking(true);
        }
        if (connect(_sock, address.SockAddr(), address.SockAddrLen()) < 0) {
            ret = -sock_errno;
            if (EINTR == -ret ||
                    ((pTimeout || origNonblocking) && (EINPROGRESS == -ret || EWOULDBLOCK == -ret || EALREADY == -ret))) {
                int32_t evts = _Select(SOCK_EVT_READ | SOCK_EVT_WRITE, pTimeout);
                if (evts > 0 && ((evts & SOCK_EVT_WRITE) || (evts & SOCK_EVT_READ))) {
                    int32_t err = 0;
                    socklen_t errLen = sizeof(err);
                    ret = _GetOption(SOL_SOCKET, SO_ERROR, &err, &errLen);
                    if (ERR_NONE == ret) {
                        ret = -err;
                    }
                }
            }
        }
        if (pTimeout) {
            _SetNonblocking(origNonblocking);
        }

        if (ERR_NONE == ret) {
            _UpdateLocalAddr();
            _UpdatePeerAddr();
        } else {
            MWL_ERR_ERRNO("connect to %s at port %d as af %s failed", -ret, address.Host(), address.Port(), address.FamilyName());
        }
        return ret;
    }

    int32_t Socket::Implement::_Accept(Socket &acceptee, const TimeSpec *pTimeout) {
        int32_t ret = ERR_NONE;
        if (pTimeout) {
            int32_t evt = _Select(SOCK_EVT_READ, pTimeout);
            if (evt < 0) {
                return evt;
            } else if (0 == evt) {
                return -ETIMEDOUT;
            }
        }
        sockaddr_storage ss;
        socklen_t addrLen = sizeof(ss);
        sockaddr *pSockAddr = reinterpret_cast<sockaddr *>(&ss);
        SockHandle sock = INVALID_SOCKET;
        do {
            sock = accept(_sock, pSockAddr, &addrLen);
        } while (sock == INVALID_SOCKET &&
                 (sock_errno == EINTR || sock_errno == ECONNABORTED || sock_errno == EPROTO || sock_errno == EWOULDBLOCK));
        if (INVALID_SOCKET == sock) {
            ret = -sock_errno;
            MWL_ERR_ERRNO("accept failed", -ret);
        } else {
            acceptee.m_pImpl->_SetHandle(sock, _af, _type, _proto);
            acceptee.SetNonblocking(_blkStB4Listening);
        }
        return ret;
    }

    int32_t Socket::Implement::_Select(uint32_t events, const TimeSpec *pTimeout) {
        SockHandle sock = _sock;
        if (sock < 0) {
            MWL_WARN("can't select on sock %d", static_cast<int32_t>(sock));
            return EINVAL;
        }
        fd_set *pReadfds = NULL, *pWritefds = NULL, *pExceptfds = NULL;
        fd_set readfds, writefds, exceptfds;
        if (events & SOCK_EVT_READ) {
            FD_ZERO(&readfds);
            pReadfds = &readfds;
            FD_SET(sock, &readfds);
        }
        if (events & SOCK_EVT_WRITE) {
            FD_ZERO(&writefds);
            pWritefds = &writefds;
            FD_SET(sock, &writefds);
        }
        if (events & SOCK_EVT_EXCEPT) {
            FD_ZERO(&exceptfds);
            pExceptfds = &exceptfds;
            FD_SET(sock, &exceptfds);
        }

        timeval *pTV = nullptr;
        timeval tv;
        if (pTimeout) {
            tv.tv_sec = pTimeout->ToI32(SECOND);
            tv.tv_usec = pTimeout->ToI32(MICROSEC) - tv.tv_sec * 1000000;
            pTV = &tv;
        }

        int32_t resultEvts = 0;
        int32_t ret = select(static_cast<int32_t>(sock) + 1, pReadfds, pWritefds, pExceptfds, pTV);
        if (ret < 0) {
            resultEvts = -sock_errno;
            MWL_WARN_ERRNO("select failed", -resultEvts);
        } else if (ret > 0) {
            if (pReadfds && FD_ISSET(sock, pReadfds)) {
                resultEvts |= SOCK_EVT_READ;
            }
            if (pWritefds && FD_ISSET(sock, pWritefds)) {
                resultEvts |= SOCK_EVT_WRITE;
            }
            if (pExceptfds && FD_ISSET(sock, pExceptfds)) {
                resultEvts |= SOCK_EVT_EXCEPT;
            }
        }
        return resultEvts;
    }

    int32_t Socket::Implement::_SendTo(
        const void *pData, size_t dataLen, int32_t flags,
        const SockAddress *pDstAddr, const TimeSpec *pTimeout, bool sendAll) {
        const sockaddr *dstAddr = pDstAddr ? pDstAddr->SockAddr() : nullptr;
        socklen_t dstAddrLen = pDstAddr ? pDstAddr->SockAddrLen() : 0;
        const char *pBuf = reinterpret_cast<const char *>(pData);
        int32_t totalBytesSent = 0;
        if (pTimeout) {
            int32_t evt = _Select(SOCK_EVT_WRITE, pTimeout);
            if (evt < 0) {
                return evt;
            } else if (0 == evt) {
                return -ETIMEDOUT;
            }
        }
#ifdef __MWL_LINUX__
        if (0 == flags) {
            flags = MSG_NOSIGNAL;
        }
#endif
        do {
#ifdef __MWL_WIN__
            int32_t n = sendto(_sock, pBuf + totalBytesSent, static_cast<int>(dataLen), flags, dstAddr, dstAddrLen);
#else
            int32_t n = sendto(_sock, pBuf + totalBytesSent, dataLen, flags, dstAddr, dstAddrLen);
#endif
            if (n > 0) {
                totalBytesSent += n;
                dataLen -= n;
            } else if (n != EINTR) {
                if (n < 0) {
                    int32_t err = sock_errno;
                    MWL_WARN_ERRNO("sendto failed", err);
                    if (0 == totalBytesSent) {
                        totalBytesSent = -err;
                    }
                }
                break;
            }
        } while (sendAll && dataLen > 0);
        return totalBytesSent;
    }

    int32_t Socket::Implement::_RecvFrom(
        void *pData, size_t dataLen, int32_t flags,
        SockAddress *pSrcAddr, const TimeSpec *pTimeout, bool recvAll) {
        char *pBuf = reinterpret_cast<char *>(pData);
        int32_t totalBytesRecv = 0;
        if (pTimeout) {
            int32_t evt = _Select(SOCK_EVT_READ, pTimeout);
            if (evt < 0) {
                return evt;
            } else if (0 == evt) {
                return -ETIMEDOUT;
            }
        }
        sockaddr_storage ss;
        sockaddr *srcAddr = nullptr;
        socklen_t srcAddrLen = 0;
        if (pSrcAddr) {
            memset(&ss, 0, sizeof(ss));
            srcAddr = reinterpret_cast<sockaddr *>(&ss);
            srcAddrLen = sizeof(ss);
        }
        do {
#ifdef __MWL_WIN__
            int32_t n = recvfrom(_sock, pBuf + totalBytesRecv, static_cast<int>(dataLen), flags, srcAddr, &srcAddrLen);
#else
            int32_t n = recvfrom(_sock, pBuf + totalBytesRecv, dataLen, flags, srcAddr, &srcAddrLen);
#endif
            if (n > 0) {
                totalBytesRecv += n;
                dataLen -= n;
            } else if (n != EINTR) {
                if (n < 0) {
                    int32_t err = sock_errno;
                    MWL_WARN_ERRNO("recvfrom failed", err);
                    if (0 == totalBytesRecv) {
                        totalBytesRecv = -err;
                    }
                }
                break;
            }
        } while (recvAll && dataLen > 0);
        if (pSrcAddr) {
            if (srcAddr->sa_family != 0) {
                pSrcAddr->SetAddress(srcAddr, srcAddrLen);
            } else {
                *pSrcAddr = _peerAddr;
            }
        }
        return totalBytesRecv;
    }

    int32_t Socket::Implement::_SetNonblocking(bool nonblocking) {
        int32_t ret = ERR_NONE;
        if (_nonblocking == nonblocking) {
            return ret;
        }
#ifdef _WIN32
        unsigned long nonBlock = nonblocking ? 1 : 0;
        if (ioctlsocket(_sock, FIONBIO, &nonBlock) < 0) {
            ret = -sock_errno;
        }
#else
        int32_t sockFlags = fcntl(_sock, F_GETFL, 0);
        if (-1 == sockFlags) {
            ret = -sock_errno;
        } else {
            if (nonblocking) {
                sockFlags |= O_NONBLOCK;
            } else {
                sockFlags &= ~O_NONBLOCK;
            }
            ret = fcntl(_sock, F_SETFL, sockFlags);
        }
#endif
        if (ret < 0) {
            MWL_WARN_ERRNO("setblocking failed", ret);
        } else {
            _nonblocking = nonblocking;
        }
        return ret;
    }

    bool Socket::Implement::_IsNonblocking() {
        return _nonblocking;
    }

    int32_t Socket::Implement::_SetOption(int32_t level, int32_t optName, const void *pOptVal, socklen_t valLen) {
        int32_t ret = ERR_NONE;
        if (setsockopt(_sock, level, optName, reinterpret_cast<const char *>(pOptVal), valLen) < 0) {
            ret = -sock_errno;
            MWL_ERR_ERRNO("setsockopt(%d, %d, %p, %d) failed", -ret, level, optName, pOptVal, valLen);
        }
        return ret;
    }

    int32_t Socket::Implement::_GetOption(int32_t level, int32_t optName, void *pOptVal, socklen_t *valLen) {
        int32_t ret = ERR_NONE;
        if (getsockopt(_sock, level, optName, reinterpret_cast<char *>(pOptVal), valLen) < 0) {
            ret = -sock_errno;
            MWL_ERR_ERRNO("getsockopt(%d, %d, %p, %d) failed", -ret, level, optName, pOptVal, *valLen);
        }
        return ret;
    }

    int32_t Socket::Implement::_UpdateLocalAddr() {
        int32_t ret = ERR_NONE;
        if (SOCK_AF_INET == _af || SOCK_AF_INET6 == _af) {
            sockaddr_storage ss;
            socklen_t addrLen = sizeof(ss);
            if (getsockname(_sock, reinterpret_cast<sockaddr *>(&ss), &addrLen) < 0) {
                ret = -sock_errno;
                MWL_ERR_ERRNO("getsockname failed", -ret);
                _localAddr.Reset();
            } else {
                _localAddr.SetAddress(reinterpret_cast<sockaddr *>(&ss), addrLen);
                _localAddr.SetFamily(_af);
            }
        }
        return ret;
    }

    int32_t Socket::Implement::_UpdatePeerAddr() {
        int32_t ret = ERR_NONE;
        if (SOCK_AF_INET == _af || SOCK_AF_INET6 == _af) {
            sockaddr_storage ss;
            socklen_t addrLen = sizeof(ss);
            if (getpeername(_sock, reinterpret_cast<sockaddr *>(&ss), &addrLen) < 0) {
                ret = -sock_errno;
                MWL_ERR_ERRNO("getpeername failed", -ret);
                _peerAddr.Reset();
            } else {
                _peerAddr.SetAddress(reinterpret_cast<sockaddr *>(&ss), addrLen);
                _peerAddr.SetFamily(_af);
            }
        }
        return ret;
    }

    const SockAddress &Socket::Implement::_LocalAddress() {
        return _localAddr;
    }

    const SockAddress &Socket::Implement::_PeerAddress() {
        return _peerAddr;
    }

}
