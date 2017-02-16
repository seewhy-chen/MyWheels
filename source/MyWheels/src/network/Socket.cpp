#include "inc/Socket.h"
#include "SocketImplement.h"

namespace mwl {

    Socket::Socket() : m_pImpl(new Implement()) {}

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

    int32_t Socket::Bind(const SockAddress &address) {
        return m_pImpl->_Bind(address);
    }

    int32_t Socket::Listen(int32_t backlog) {
        return m_pImpl->_Listen(backlog);
    }

    int32_t Socket::Connect(const SockAddress &address, const TimeSpec &timeout) {
        return m_pImpl->_Connect(address, timeout);
    }

    SharedPtr<Socket> Socket::Accept(const TimeSpec &timeout) {
        return m_pImpl->_Accept(timeout);
    }

    uint32_t Socket::Select(uint32_t events, const TimeSpec &timeout) {
        return m_pImpl->_Select(events, timeout);
    }

    int32_t Socket::Send(const void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return m_pImpl->_SendTo(pData, dataLen, NULL, timeout, false);
    }

    int32_t Socket::SendAll(const void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return m_pImpl->_SendTo(pData, dataLen, NULL, timeout, true);
    }

    int32_t Socket::SendTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec &timeout) {
        return m_pImpl->_SendTo(pData, dataLen, &dstAddr, timeout, false);
    }

    int32_t Socket::SendAllTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec &timeout) {
        return m_pImpl->_SendTo(pData, dataLen, &dstAddr, timeout, true);
    }

    int32_t Socket::Recv(void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, NULL, timeout, false);
    }

    int32_t Socket::RecvAll(void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, NULL, timeout, true);
    }

    int32_t Socket::RecvFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec &timeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, &srcAddr, timeout, false);
    }

    int32_t Socket::RecvAllFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec &timeout) {
        return m_pImpl->_RecvFrom(pData, dataLen, &srcAddr, timeout, true);
    }

    int32_t Socket::SetNonblock(bool nonblock) {
        return m_pImpl->_SetNonblock(nonblock);
    }

    bool Socket::IsNonblock() const {
        return m_pImpl->_IsNonblock();
    }

    int32_t Socket::SetOption(int32_t level, int32_t optName, const void *pOptVal, int32_t valLen) {
        return m_pImpl->_SetOption(level, optName, pOptVal, valLen);
    }

    int32_t Socket::GetOption(int32_t level, int32_t optName, void *pOptVal, int32_t valLen) const {
        return m_pImpl->_GetOption(level, optName, pOptVal, valLen);
    }

    const SockAddress &Socket::LocalAddress() const {
        return m_pImpl->_LocalAddress();
    }

    const SockAddress &Socket::PeerAddress() const {
        return m_pImpl->_PeerAddress();
    }

    void SockGetHostByName() {
    }

    void SockGetHostByAddr() {
    }

    void SockGetServByName() {
    }

    void SockGetAddrInfo() {
    }

    SockSelector::SockSelector() {
    }

    SockSelector::~SockSelector() {
    }

    int32_t SockSelector::AddSocket(Socket *pSock, uint32_t events, SockEventHandler evtHandler, void *pData) {
        return -1;
    }

    int32_t SockSelector::RemoveSocket(const Socket *pSock) {
        return -1;
    }

    int32_t SockSelector::Select(uint32_t events, const TimeSpec &timeout) {
        return -1;
    }

}
