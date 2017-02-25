#include "inc/Socket.h"
#include "SocketImplement.h"

namespace mwl {

    Socket::Socket() : m_pImpl(new Implement()) {}

    Socket::Socket(SockAddressFamily af, SockType type, SockProtocol protocol) 
    : m_pImpl(new Implement()) {
        m_pImpl->_Open(af, type, protocol);
    }

    Socket::~Socket() {
        delete m_pImpl;
    }

    int32_t Socket::Open(SockAddressFamily af, SockType type, SockProtocol protocol) {
        return m_pImpl->_Open(af, type, protocol);
    }

    int32_t Socket::Reopen() {
        return m_pImpl->_Reopen();
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

    int32_t Socket::Bind(const char *host, int32_t port) {
        return m_pImpl->_Bind(SockAddress(host, port, static_cast<SockAddressFamily>(m_pImpl->_af)));
    }

    int32_t Socket::Bind(const char *host, const char *service) {
        return m_pImpl->_Bind(SockAddress(host, service, static_cast<SockAddressFamily>(m_pImpl->_af)));
    }

    int32_t Socket::Listen(int32_t backlog) {
        return m_pImpl->_Listen(backlog);
    }

    int32_t Socket::Connect(const SockAddress &address, const TimeSpec *pTimeout) {
        return m_pImpl->_Connect(address, pTimeout);
    }

    int32_t Socket::Connect(const char *host, int32_t port, const TimeSpec *pTimeout) {
        return m_pImpl->_Connect(SockAddress(host, port, m_pImpl->_af), pTimeout);
    }

    int32_t Socket::Connect(const char *host, const char *service, const TimeSpec *pTimeout) {
        return m_pImpl->_Connect(SockAddress(host, service, m_pImpl->_af), pTimeout);
    }

    int32_t Socket::Accept(Socket &acceptee, const TimeSpec *pTimeout) {
        return m_pImpl->_Accept(acceptee, pTimeout);
    }
    
    int32_t Socket::Select(uint32_t events, const TimeSpec *pTimeout) {
        return m_pImpl->_Select(events, pTimeout);
    }

    int32_t Socket::Send(const void *pData, int32_t dataLen, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, NULL, pTimeout, false);
    }

    int32_t Socket::SendAll(const void *pData, int32_t dataLen, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, NULL, pTimeout, true);
    }

    int32_t Socket::SendTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, &dstAddr, pTimeout, false);
    }

    int32_t Socket::SendAllTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, &dstAddr, pTimeout, true);
    }

    int32_t Socket::Recv(void *pData, int32_t dataLen, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, NULL, pTimeout, false);
    }

    int32_t Socket::RecvAll(void *pData, int32_t dataLen, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, NULL, pTimeout, true);
    }

    int32_t Socket::RecvFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, &srcAddr, pTimeout, false);
    }

    int32_t Socket::RecvAllFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec *pTimeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, &srcAddr, pTimeout, true);
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

}
