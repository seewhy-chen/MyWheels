#include "inc/Socket.h"
#include "SockAddressImplement.h"
#include "InternalCommon.h"

namespace mwl {

    SockAddress::SockAddress(const char *host, const char *service, SockAddressFamily af) 
    : m_pImpl(new Implement(host, service, af)) {}

    SockAddress::SockAddress(const char *host, int32_t port, SockAddressFamily af) 
    : m_pImpl(new Implement(host, port, af)) {}

    SockAddress::SockAddress(const sockaddr *pSockAddr, socklen_t addrLen)
    : m_pImpl(new Implement(pSockAddr, addrLen)) {}

    SockAddress::SockAddress(const SockAddress &src)
    : m_pImpl(new Implement(*src.m_pImpl)) {}

    SockAddress::~SockAddress() {
        delete m_pImpl;
    }

    SockAddress &SockAddress::operator=(const SockAddress &rhs) {
        if (this != &rhs) {
            m_pImpl->_SetAddress(rhs.m_pImpl->_host.c_str(), rhs.m_pImpl->_service.c_str(), rhs.m_pImpl->_af);
        }
        return *this;
    }

    int32_t SockAddress::SetAddress(const char *host, const char *service, SockAddressFamily af) {
        return m_pImpl->_SetAddress(host, service, af);
    }

    int32_t SockAddress::SetAddress(const char *host, int32_t port, SockAddressFamily af) {
        return m_pImpl->_SetAddress(host, port, af);
    }

    int32_t SockAddress::SetAddress(const sockaddr *pSockAddr, socklen_t addrLen) {
        return m_pImpl->_SetAddress(pSockAddr, addrLen);
    }

    int32_t SockAddress::SetHost(const char *host) {
        return m_pImpl->_SetHost(host);
    }

    int32_t SockAddress::SetHost(const sockaddr *pSockAddr, socklen_t addrLen) {
        return m_pImpl->_SetHost(pSockAddr, addrLen);
    }

    int32_t SockAddress::SetPort(int32_t port) {
        return m_pImpl->_SetPort(port);
    }

    int32_t SockAddress::SetPort(const sockaddr *pSockAddr, socklen_t addrLen) {
        return m_pImpl->_SetPort(pSockAddr, addrLen);
    }

    int32_t SockAddress::SetFamily(SockAddressFamily af) {
        return m_pImpl->_SetFamily(af);
    }

    int32_t SockAddress::SetFamily(const sockaddr *pSockAddr, socklen_t addrLen) {
        return m_pImpl->_SetFamily(pSockAddr, addrLen);
    }

    int32_t SockAddress::Resolve() {
        return m_pImpl->_Resolve();
    }

    void SockAddress::Reset() {
        return m_pImpl->_Reset();
    }

    const char *SockAddress::Host() const {
        return m_pImpl->_host.c_str();
    }

    int32_t SockAddress::Port() const {
        return m_pImpl->_port;
    }

    SockAddressFamily SockAddress::Family() const {
        return m_pImpl->_af;
    }

    const sockaddr *SockAddress::SockAddr() const {
        return m_pImpl->_SockAddr();
    }

    const socklen_t SockAddress::SockAddrLen() const {
        return m_pImpl->_SockAddrLen();
    }

    void SockAddress::Swap(SockAddress &other) {
        std::swap(m_pImpl, other.m_pImpl);
    }

}
