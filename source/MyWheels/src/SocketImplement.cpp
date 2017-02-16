#include "SocketImplement.h"
#include "SockInternalUtils.h"

namespace mwl {

    Socket::Implement::Implement() {
        _fd = -1;
        _isOpened = false;
    }

    Socket::Implement::~Implement() {}

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
            _fd = socket(s_afMap[af], s_typeMap[type], s_protoMap[protocol]);
            if (_fd < 0) {
                int32_t err = errno;
                MWL_ERROR_ERRNO("open socket(%d, %d, %d) failed", err, af, type, protocol);
                ret = -err;
            } else {
                _isOpened = true;
            }
        }
        return ret;
    }

    int32_t Socket::Implement::_Shutdown(int32_t how) {
        if (how < 0 || how >= SockShutdownCount) {
            MWL_WARN("invalid how: %d", how);
            return ERR_INVAL_PARAM;
        }

        int32_t ret = shutdown(_fd, s_shutdownMap[how]);
        if (ret < 0) {
            int32_t err = errno;
            MWL_ERROR_ERRNO("shutdown failed", err);
            ret = -err;
        }
        return ret;
    }

    int32_t Socket::Implement::_Close() {
        int32_t ret = ERR_NONE;
        if (_isOpened) {
            ret = closesocket(_fd);
            if (ret < 0) {
                int32_t err = errno;
                MWL_ERROR_ERRNO("close socket of fd %d failed", err, _fd);
                ret = -err;
            } else {
                _isOpened = false;
            }
        }
        return ret;
    }

    int32_t Socket::Implement::_Bind(const SockAddress &address) {
        int32_t ret = bind(_fd, address.SockAddr(), address.SockAddrLen());
        if (ret < 0) {
            int32_t err = errno;
            MWL_ERROR_ERRNO("bind to %s at port %d as af %d failed", err, address.Host(), address.Port(), address.Family());
            ret = -errno;
        } else {
            _localAddr = address;
        }
        return ret;
    }

    int32_t Socket::Implement::_Listen(int32_t backlog) {
        int32_t ret = listen(_fd, backlog);
        if (ret < 0) {
            int32_t err = errno;
            MWL_ERROR_ERRNO("listen on %s at port %d as af %d failed", err, 
                _localAddr.Host(), _localAddr.Port(), _localAddr.Family());
            ret = -errno;
        }
        return ret;
    }

    int32_t Socket::Implement::_Connect(const SockAddress &address, const TimeSpec &timeout) {
        int32_t ret = connect(_fd, address.SockAddr(), address.SockAddrLen());
        if (ret < 0) {
            int32_t err = errno;
            MWL_ERROR_ERRNO("connect to %s at port %d as af %d failed", err, address.Host(), address.Port(), address.Family());
            ret = -errno;
        } else {
            _peerAddr = address;
        }
        return -1;
    }

    SharedPtr<Socket> Socket::Implement::_Accept(const TimeSpec &timeout) {
        return SharedPtr<Socket>();
    }

    uint32_t Socket::Implement::_Select(uint32_t events, const TimeSpec &timeout) {
        return 0;
    }

    int32_t Socket::Implement::_SendTo(const void *pData, int32_t dataLen, const SockAddress *pDstAddr, const TimeSpec &timeout, bool sendAll) {
        return -1;
    }

    int32_t Socket::Implement::_RecvFrom(void *pData, int32_t dataLen, SockAddress *pSrcAddr, const TimeSpec &timeout, bool recvAll) {
        return -1;
    }

    int32_t Socket::Implement::_SetNonblock(bool nonblock) {
        return -1;
    }

    bool Socket::Implement::_IsNonblock() const {
        return false;
    }

    int32_t Socket::Implement::_SetOption(int32_t level, int32_t optName, const void *pOptVal, int32_t valLen) {
        return -1;
    }

    int32_t Socket::Implement::_GetOption(int32_t level, int32_t optName, void *pOptVal, int32_t valLen) {
        return -1;
    }

    const SockAddress &Socket::Implement::_LocalAddress() const {
        return _localAddr;
    }

    const SockAddress &Socket::Implement::_PeerAddress() const {
        return _peerAddr;
    }

}
