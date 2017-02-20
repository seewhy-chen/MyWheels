#include "SocketImplement.h"
#include "SockInternalUtils.h"

namespace mwl {

    Socket::Implement::Implement() {
        _initializeSock();
        _sock = INVALID_SOCKET;
        _af = SOCK_AF_INVALID;
        _type = SOCK_TYPE_INVALID;
        _proto = SOCK_PROTO_INVALID;
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
                MWL_ERROR_ERRNO("open socket(%d, %d, %d) failed", -ret, af, type, protocol);
            } else {
                _af = static_cast<SockAddressFamily>(af);
                _type = static_cast<SockType>(type);
                _proto = static_cast<SockProtocol>(protocol);
            }
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
            MWL_ERROR_ERRNO("shutdown failed", -ret);
        }
        return ret;
    }

    int32_t Socket::Implement::_Close() {
        int32_t ret = ERR_NONE;
        if (_isOpened) {
            ret = closesocket(_sock);
            if (ret < 0) {
                ret = -sock_errno;
                MWL_ERROR_ERRNO("close sock failed", -ret);
            } else {
#ifdef __MWL_LINUX__
                if (_af == SOCK_AF_LOCAL) {
                    unlink(_localAddr.Host());
                }
#endif
                _isOpened = false;
                _sock = INVALID_SOCKET;
                _af = SOCK_AF_INVALID;
                _type = SOCK_TYPE_INVALID;
                _proto = SOCK_PROTO_INVALID;
            }
            _af = SOCK_AF_INVALID;
            _type = SOCK_TYPE_INVALID;
            _proto = SOCK_TYPE_INVALID;
        }
        return ret;
    }

    int32_t Socket::Implement::_SetHandle(SockHandle handle) {
        _Close();
        _sock = handle;
        int32_t sockType;
        socklen_t optLen = sizeof(sockType);
        int32_t ret = _GetOption(SOL_SOCKET, SO_TYPE, &sockType, &optLen);
        if (ret < 0) {
            return ret;
        }
        switch (sockType) {
        case SOCK_STREAM:
            _type = SOCK_TYPE_STREAM;
            break;
        case SOCK_DGRAM:
            _type = SOCK_TYPE_DGRAM;
            break;
        case SOCK_SEQPACKET:
            _type = SOCK_TYPE_SEQPKT;
            break;
        default: {
                MWL_WARN("un-support sock type %d", sockType);
                return ERR_INVAL_PARAM;
            }
            break;
        }
        _af = SOCK_AF_UNSPEC;
        _proto = SOCK_PROTO_DEFAULT;
        _UpdateLocalAddr();
        _UpdatePeerAddr();
        return ERR_NONE;
    }

    SockHandle Socket::Implement::_Handle() {
        return _sock;
    }

    int32_t Socket::Implement::_Bind(const SockAddress &address) {
        int32_t ret = ERR_NONE;
        if (bind(_sock, address.SockAddr(), address.SockAddrLen()) < 0) {
            ret = -sock_errno;
            MWL_ERROR_ERRNO("bind to %s at port %d as af %d failed", -ret, address.Host(), address.Port(), address.Family());
            ret = -sock_errno;
        } else {
            _UpdateLocalAddr();
        }
        return ret;
    }

    int32_t Socket::Implement::_Listen(int32_t backlog) {
        int32_t ret = ERR_NONE;
        if (listen(_sock, backlog) < 0) {
            ret = -sock_errno;
            MWL_ERROR_ERRNO("listen on %s at port %d as af %d failed",
                            -ret, _localAddr.Host(), _localAddr.Port(), _localAddr.Family());
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
            ret = -errno;
            if (pTimeout && (EINPROGRESS == -ret || EWOULDBLOCK == -ret || EALREADY == -ret)) {
                int32_t evts = _Select(SOCK_EVT_READ | SOCK_EVT_WRITE, pTimeout);
                if ((evts & SOCK_EVT_WRITE) || (evts & SOCK_EVT_READ)) {
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
            MWL_ERROR_ERRNO("connect to %s at port %d as af %d failed", -ret, address.Host(), address.Port(), address.Family());
        }
        return ret;
    }

    SharedPtr<Socket> Socket::Implement::_Accept(const TimeSpec *pTimeout) {
        SharedPtr<Socket> acptSock;
        if (_Select(SOCK_EVT_READ, pTimeout) & SOCK_EVT_READ) {
            sockaddr_storage ss;
            socklen_t addrLen = sizeof(ss);
            sockaddr *pSockAddr = reinterpret_cast<sockaddr *>(&ss);
            SockHandle sock = accept(_sock, pSockAddr, &addrLen);
            if (INVALID_SOCKET == sock) {
                MWL_ERROR_ERRNO("accept failed", sock_errno);
            } else {
                acptSock.reset(new Socket(sock));
            }
        }
        return acptSock;
    }

    int32_t Socket::Implement::_Select(uint32_t events, const TimeSpec *pTimeout) {
        if (_sock < 0) {
            MWL_WARN("can't select on sock %d", _sock);
            return EINVAL;
        }
        fd_set *pReadfds = NULL, *pWritefds = NULL, *pExceptfds = NULL;
        fd_set readfds, writefds, exceptfds;
        if (events & SOCK_EVT_READ) {
            FD_ZERO(&readfds);
            pReadfds = &readfds;
            FD_SET(_sock, &readfds);
        }
        if (events & SOCK_EVT_WRITE) {
            FD_ZERO(&writefds);
            pWritefds = &writefds;
            FD_SET(_sock, &writefds);
        }
        if (events & SOCK_EVT_EXCEPT) {
            FD_ZERO(&exceptfds);
            pExceptfds = &exceptfds;
            FD_SET(_sock, &exceptfds);
        }

        timeval *pTV = nullptr;
        timeval tv;
        if (pTimeout) {
            tv.tv_sec = pTimeout->ToI32(SECOND);
            tv.tv_usec = pTimeout->ToI32(MICROSEC) - tv.tv_sec * 1000000;
            pTV = &tv;
        }

        int32_t resultEvts = 0;
        int32_t ret = select(static_cast<int32_t>(_sock) + 1, pReadfds, pWritefds, pExceptfds, pTV);
        if (ret < 0) {
            resultEvts = -sock_errno;
            MWL_WARN_ERRNO("select failed", -resultEvts);
        } else if (ret > 0) {
            if (pReadfds && FD_ISSET(_sock, pReadfds)) {
                resultEvts |= SOCK_EVT_READ;
            }
            if (pWritefds && FD_ISSET(_sock, pWritefds)) {
                resultEvts |= SOCK_EVT_WRITE;
            }
            if (pExceptfds && FD_ISSET(_sock, pExceptfds)) {
                resultEvts |= SOCK_EVT_EXCEPT;
            }
        }
        return resultEvts;
    }

    int32_t Socket::Implement::_SendTo(
        const void * /*pData*/,
        int32_t /*dataLen*/,
        const SockAddress * /*pDstAddr*/,
        const TimeSpec * /*pTimeout*/,
        bool /*sendAll*/) {
        return -1;
    }

    int32_t Socket::Implement::_RecvFrom(
        void * /*pData*/,
        int32_t /*dataLen*/,
        SockAddress * /*pSrcAddr*/,
        const TimeSpec * /*pTimeout*/,
        bool /*recvAll*/) {
        return -1;
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
            MWL_ERROR_ERRNO("setsockopt(%d, %d, %p, %d) failed", -ret, level, optName, pOptVal, valLen);
        }
        return ret;
    }

    int32_t Socket::Implement::_GetOption(int32_t level, int32_t optName, void *pOptVal, socklen_t *valLen) {
        int32_t ret = ERR_NONE;
        if (getsockopt(_sock, level, optName, reinterpret_cast<char *>(pOptVal), valLen) < 0) {
            ret = -sock_errno;
            MWL_ERROR_ERRNO("getsockopt(%d, %d, %p, %d) failed", -ret, level, optName, pOptVal, *valLen);
        }
        return ret;
    }

    int32_t Socket::Implement::_UpdateLocalAddr() {
        sockaddr_storage ss;
        socklen_t addrLen = sizeof(ss);
        int32_t ret = ERR_NONE;
        if (getsockname(_sock, reinterpret_cast<sockaddr *>(&ss), &addrLen) < 0) {
            ret = -sock_errno;
            MWL_ERROR_ERRNO("getsockname failed", -ret);
            _localAddr.Reset();
        } else {
            _localAddr.SetAddress(reinterpret_cast<sockaddr *>(&ss), addrLen);
        }
        return ret;
    }

    int32_t Socket::Implement::_UpdatePeerAddr() {
        sockaddr_storage ss;
        socklen_t addrLen = sizeof(ss);
        int32_t ret = ERR_NONE;
        if (getpeername(_sock, reinterpret_cast<sockaddr *>(&ss), &addrLen) < 0) {
            ret = -sock_errno;
            MWL_ERROR_ERRNO("getpeername failed", -ret);
            _peerAddr.Reset();
        } else {
            _peerAddr.SetAddress(reinterpret_cast<sockaddr *>(&ss), addrLen);
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
