#include "inc/Socket.h"
#include "SockAddressImplement.h"
#include "InternalCommon.h"

namespace mwl {

    SockAddress::SockAddress(const char *host, const char *service, SockAddressFamily af) 
    : m_pImpl(new Implement(host, service, af)) {}

    SockAddress::SockAddress(const char *host, int32_t port, SockAddressFamily af) 
    : m_pImpl(new Implement(host, port, af)) {}

    SockAddress::SockAddress(const sockaddr *pSockAddr)
    : m_pImpl(new Implement(pSockAddr)) {}

    SockAddress::SockAddress(const SockAddress &src) 
    : m_pImpl(new Implement(src.RawAddr())) {}

    SockAddress::~SockAddress() {
        delete m_pImpl;
    }

    SockAddress &SockAddress::operator=(const SockAddress &rhs) {
        if (this != &rhs) {
            m_pImpl->_SetAddress(rhs.RawAddr());
        }
        return *this;
    }

    int32_t SockAddress::SetAddress(const char *host, const char *service, SockAddressFamily af) {
        return m_pImpl->_SetAddress(host, service, af);
    }

    int32_t SockAddress::SetAddress(const char *host, int32_t port, SockAddressFamily af) {
        char service[64] = {0};
        snprintf(service, sizeof(service), "%d", port);
        return m_pImpl->_SetAddress(host, service, af);
    }

    int32_t SockAddress::SetAddress(const sockaddr *pSockAddr) {
        return m_pImpl->_SetAddress(pSockAddr);
    }

    int32_t SockAddress::SetHost(const char *host) {
        return m_pImpl->_SetHost(host);
    }

    int32_t SockAddress::SetHost(const sockaddr *pSockAddr) {
        return m_pImpl->_SetHost(pSockAddr);
    }

    int32_t SockAddress::SetPort(int32_t port) {
        return m_pImpl->_SetPort(port);
    }

    int32_t SockAddress::SetPort(const sockaddr *pSockAddr) {
        return m_pImpl->_SetPort(pSockAddr);
    }

    int32_t SockAddress::SetFamily(SockAddressFamily af) {
        return m_pImpl->_SetFamily(af);
    }

    int32_t SockAddress::SetFamily(const sockaddr *pSockAddr) {
        return m_pImpl->_SetFamily(pSockAddr);
    }

    void SockAddress::Swap(SockAddress &other) {
        std::swap(m_pImpl, other.m_pImpl);
    }

    const sockaddr *SockAddress::RawAddr() const {
        return m_pImpl->_rawAddr;
    }
}
