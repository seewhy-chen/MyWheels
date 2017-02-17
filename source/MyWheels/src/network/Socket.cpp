#include "inc/Socket.h"
#include "SocketImplement.h"

namespace mwl {

    Socket::Socket() : m_pImpl(new Implement()) {}

    Socket::Socket(SockHandle handle) : m_pImpl(new Implement()) {
        m_pImpl->_SetHandle(handle);
    }

    Socket::Socket(SockAddressFamily family, SockType type, SockProtocol protocol) 
    : m_pImpl(new Implement()) {
        m_pImpl->_Open(family, type, protocol);
    }

    Socket::~Socket() {
        delete m_pImpl;
    }

    int32_t Socket::Open(SockAddressFamily family, SockType type, SockProtocol protocol) {
        return m_pImpl->_Open(family, type, protocol);
    }

    int32_t Socket::Shutdown(SockShutdown how) {
        return m_pImpl->_Shutdown(how);
    }

    int32_t Socket::Close() {
        return m_pImpl->_Close();
    }

    int32_t Socket::SetHandle(SockHandle handle) {
        return m_pImpl->_SetHandle(handle);
    }

    SockHandle Socket::Handle() const {
        return m_pImpl->_Handle();
    }

    int32_t Socket::Bind(const SockAddress &address) {
        return m_pImpl->_Bind(address);
    }

    int32_t Socket::Listen(int32_t backlog) {
        return m_pImpl->_Listen(backlog);
    }

    int32_t Socket::Connect(const SockAddress &address, const TimeSpec *pTimeout) {
        return m_pImpl->_Connect(address, pTimeout);
    }

    SharedPtr<Socket> Socket::Accept(const TimeSpec *pTimeout) {
        return m_pImpl->_Accept(pTimeout);
    }

    int32_t Socket::Select(uint32_t events, const TimeSpec *pTimeout) {
        return m_pImpl->_Select(events, pTimeout);
    }

    int32_t Socket::Send(const void *pData, int32_t dataLen, const TimeSpec *pTimeout) {
        return m_pImpl->_SendTo(pData, dataLen, NULL, pTimeout, false);
    }

    int32_t Socket::SendAll(const void *pData, int32_t dataLen, const TimeSpec *pTimeout) {
        return m_pImpl->_SendTo(pData, dataLen, NULL, pTimeout, true);
    }

    int32_t Socket::SendTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec *pTimeout) {
        return m_pImpl->_SendTo(pData, dataLen, &dstAddr, pTimeout, false);
    }

    int32_t Socket::SendAllTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec *pTimeout) {
        return m_pImpl->_SendTo(pData, dataLen, &dstAddr, pTimeout, true);
    }

    int32_t Socket::Recv(void *pData, int32_t dataLen, const TimeSpec *pTimeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, NULL, pTimeout, false);
    }

    int32_t Socket::RecvAll(void *pData, int32_t dataLen, const TimeSpec *pTimeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, NULL, pTimeout, true);
    }

    int32_t Socket::RecvFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec *pTimeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, &srcAddr, pTimeout, false);
    }

    int32_t Socket::RecvAllFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec *pTimeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, &srcAddr, pTimeout, true);
    }

    int32_t Socket::SetNonblocking(bool nonblocking) {
        return m_pImpl->_SetNonblocking(nonblocking);
    }

    bool Socket::IsNonblocking() const {
        return m_pImpl->_IsNonblocking();
    }

    int32_t Socket::SetOption(int32_t level, int32_t optName, const void *pOptVal, socklen_t valLen) {
        return m_pImpl->_SetOption(level, optName, pOptVal, valLen);
    }

    int32_t Socket::GetOption(int32_t level, int32_t optName, void *pOptVal, socklen_t *valLen) const {
        return m_pImpl->_GetOption(level, optName, pOptVal, valLen);
    }

    const SockAddress &Socket::LocalAddress() const {
        return m_pImpl->_LocalAddress();
    }

    const SockAddress &Socket::PeerAddress() const {
        return m_pImpl->_PeerAddress();
    }

    //void SockGetHostByName() {
    //}

    //void SockGetHostByAddr() {
    //}

    //void SockGetServByName() {
    //}

    //void SockGetAddrInfo() {
    //}

    //SockSelector::SockSelector() {
    //}

    //SockSelector::~SockSelector() {
    //}

    //int32_t SockSelector::AddSocket(Socket *pSock, uint32_t events, SockEventHandler evtHandler, void *pData) {
    //    return -1;
    //}

    //int32_t SockSelector::RemoveSocket(const Socket *pSock) {
    //    return -1;
    //}

    //int32_t SockSelector::Select(uint32_t events, const TimeSpec *pTimeout) {
    //    return -1;
    //}

}
