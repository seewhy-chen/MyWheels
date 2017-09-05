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

    int32_t Socket::Bind(const String &host, int32_t port) {
        return m_pImpl->_Bind(SockAddress(host, port, static_cast<SockAddressFamily>(m_pImpl->_af)));
    }

    int32_t Socket::Bind(const String &host, const String &service) {
        return m_pImpl->_Bind(SockAddress(host, service, static_cast<SockAddressFamily>(m_pImpl->_af)));
    }

    int32_t Socket::Listen(int32_t backlog) {
        return m_pImpl->_Listen(backlog);
    }

    int32_t Socket::Connect(const SockAddress &address) {
        return m_pImpl->_Connect(address, nullptr);
    }

    int32_t Socket::Connect(const SockAddress &address, const TimeSpan &timeout) {
        return m_pImpl->_Connect(address, &timeout);
    }

    int32_t Socket::Connect(const String &host, int32_t port) {
        return m_pImpl->_Connect(SockAddress(host, port, m_pImpl->_af), nullptr);
    }

    int32_t Socket::Connect(const String &host, const TimeSpan &timeout) {
        return m_pImpl->_Connect(SockAddress(host, 0, m_pImpl->_af), &timeout);
    }

    int32_t Socket::Connect(const String &host, int32_t port, const TimeSpan &timeout) {
        return m_pImpl->_Connect(SockAddress(host, port, m_pImpl->_af), &timeout);
    }

    int32_t Socket::Connect(const String &host, const String &service) {
        return m_pImpl->_Connect(SockAddress(host, service, m_pImpl->_af), nullptr);
    }

    int32_t Socket::Connect(const String &host, const String &service, const TimeSpan &timeout) {
        return m_pImpl->_Connect(SockAddress(host, service, m_pImpl->_af), &timeout);
    }

    int32_t Socket::Accept(Socket &acceptee) {
        return m_pImpl->_Accept(acceptee, nullptr);
    }

    int32_t Socket::Accept(Socket &acceptee, const TimeSpan &timeout) {
        return m_pImpl->_Accept(acceptee, &timeout);
    }

    int32_t Socket::Select(uint32_t events) {
        return m_pImpl->_Select(events, nullptr);
    }

    int32_t Socket::Select(uint32_t events, const TimeSpan &timeout) {
        return m_pImpl->_Select(events, &timeout);
    }

    int32_t Socket::Send(const void *pData, size_t dataLen, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, nullptr, nullptr, false);
    }

    int32_t Socket::Send(const void *pData, size_t dataLen, const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, nullptr, &timeout, false);
    }

    int32_t Socket::SendAll(const void *pData, size_t dataLen, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, nullptr, nullptr, true);
    }

    int32_t Socket::SendAll(const void *pData, size_t dataLen, const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, nullptr, &timeout, true);
    }

    int32_t Socket::SendTo(const void *pData, size_t dataLen, const SockAddress &dstAddr, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, &dstAddr, nullptr, false);
    }

    int32_t Socket::SendTo(const void *pData, size_t dataLen, const SockAddress &dstAddr,
        const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, &dstAddr, &timeout, false);
    }

    int32_t Socket::SendAllTo(const void *pData, size_t dataLen, const SockAddress &dstAddr, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, &dstAddr, nullptr, true);
    }

    int32_t Socket::SendAllTo(const void *pData, size_t dataLen, const SockAddress &dstAddr,
        const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_SendTo(pData, dataLen, flags, &dstAddr, &timeout, true);
    }

    int32_t Socket::Recv(void *pData, size_t dataLen, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, nullptr, nullptr, false);
    }

    int32_t Socket::Recv(void *pData, size_t dataLen, const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, nullptr, &timeout, false);
    }

    int32_t Socket::RecvAll(void *pData, size_t dataLen, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, nullptr, nullptr, true);
    }

    int32_t Socket::RecvAll(void *pData, size_t dataLen, const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, nullptr, &timeout, true);
    }

    int32_t Socket::RecvFrom(void *pData, size_t dataLen, SockAddress &srcAddr, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, &srcAddr, nullptr, false);
    }

    int32_t Socket::RecvFrom(void *pData, size_t dataLen, SockAddress &srcAddr, const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, &srcAddr, &timeout, false);
    }

    int32_t Socket::RecvAllFrom(void *pData, size_t dataLen, SockAddress &srcAddr, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, &srcAddr, nullptr, true);
    }

    int32_t Socket::RecvAllFrom(void *pData, size_t dataLen, SockAddress &srcAddr, const TimeSpan &timeout, int32_t flags) {
        return m_pImpl->_RecvFrom(pData, dataLen, flags, &srcAddr, &timeout, true);
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
