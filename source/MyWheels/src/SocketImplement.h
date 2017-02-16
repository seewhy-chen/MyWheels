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

        int32_t _Bind(const SockAddress &address);
        int32_t _Listen(int32_t backlog);
        int32_t _Connect(const SockAddress &address, const TimeSpec &timeout);
        SharedPtr<Socket> _Accept(const TimeSpec &timeout);
        uint32_t _Select(uint32_t events, const TimeSpec &timeout);

        int32_t _SendTo(const void *pData, int32_t dataLen, const SockAddress *pDstAddr, const TimeSpec &timeout, bool sendAll);
        int32_t _RecvFrom(void *pData, int32_t dataLen, SockAddress *pSrcAddr, const TimeSpec &timeout, bool recvAll);

        int32_t _SetNonblock(bool nonblock);
        bool _IsNonblock() const;
        int32_t _SetOption(int32_t level, int32_t optName, const void *pOptVal, int32_t valLen);
        int32_t _GetOption(int32_t level, int32_t optName, void *pOptVal, int32_t valLen);

        const SockAddress &_LocalAddress() const;
        const SockAddress &_PeerAddress() const;

        uintptr_t _fd;
        bool _isOpened;
        SockAddress _localAddr;
        SockAddress _peerAddr;
    };

}

#endif // __MWL_SOCKET_IMPLEMENT_H__