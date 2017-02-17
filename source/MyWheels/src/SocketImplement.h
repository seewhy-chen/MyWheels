#ifndef __MWL_SOCKET_IMPLEMENT_H__
#define __MWL_SOCKET_IMPLEMENT_H__

#include "inc/Socket.h"

namespace mwl {

    struct Socket::Implement {
        Implement();
        ~Implement();

        int32_t _Open(int32_t af, int32_t type, int32_t protocol);
        int32_t _Shutdown(int32_t how);
        int32_t _Close();
        int32_t _SetHandle(SockHandle handle);
        SockHandle _Handle() ;

        int32_t _Bind(const SockAddress &address);
        int32_t _Listen(int32_t backlog);
        int32_t _Connect(const SockAddress &address, const TimeSpec *pTimeout);
        SharedPtr<Socket> _Accept(const TimeSpec *pTimeout);
        int32_t _Select(uint32_t events, const TimeSpec *pTimeout);

        int32_t _SendTo(const void *pData, int32_t dataLen, const SockAddress *pDstAddr, const TimeSpec *pTimeout, bool sendAll);
        int32_t _RecvFrom(void *pData, int32_t dataLen, SockAddress *pSrcAddr, const TimeSpec *pTimeout, bool recvAll);

        int32_t _SetNonblocking(bool nonblocking);
        bool _IsNonblocking();
        int32_t _SetOption(int32_t level, int32_t optName, const void *pOptVal, socklen_t valLen);
        int32_t _GetOption(int32_t level, int32_t optName, void *pOptVal, socklen_t *valLen);

        int32_t _UpdateLocalAddr();
        int32_t _UpdatePeerAddr();
        const SockAddress &_LocalAddress();
        const SockAddress &_PeerAddress();

        SockHandle _sock;
        bool _isOpened;
        int32_t _af;
        int32_t _type;
        int32_t _proto;
        bool _nonblocking;
        SockAddress _localAddr;
        SockAddress _peerAddr;
    };

}

#endif // __MWL_SOCKET_IMPLEMENT_H__