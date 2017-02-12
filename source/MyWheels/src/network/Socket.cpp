#include "inc/Socket.h"

namespace mwl {

    bool SockAddress::IsSameHost(const char * strHost) const {
        return false;
    }

    bool SockAddress::IsSamePort(int32_t port) const {
        return false;
    }

    bool SockAddress::IsSamePort(const SockAddress & rhs) const {
        return false;
    }

    bool SockAddress::operator==(const SockAddress & rhs) const {
        return false;
    }

    void SockAddress::Swap(SockAddress & other) {
    }

    SockAddress::SockAddress(const char * strAddr) {
    }

    SockAddress::SockAddress(const sockaddr * pSockAddr) {
    }

    SockAddress::SockAddress(const SockAddress & src) {
    }

    SockAddress::~SockAddress() {
    }

    SockAddress & SockAddress::operator=(const SockAddress & rhs) {
        return *this;
    }

    int32_t SockAddress::SetAddress(const char * strAddr) {
        return int32_t();
    }

    int32_t SockAddress::SetAddress(const sockaddr * pSockAddr) {
        return int32_t();
    }

    int32_t SockAddress::SetHost(const char * strHost) {
        return int32_t();
    }
    
    int32_t SockAddress::SetHost(const sockaddr * pSockAddr) {
        return int32_t();
    }

    int32_t SockAddress::SetPort(int32_t port) {
        return int32_t();
    }

    int32_t SockAddress::SetPort(const sockaddr * pSockAddr) {
        return int32_t();
    }

    const char * SockAddress::Address() const {
        return nullptr;
    }

    const char * SockAddress::Host() const {
        return nullptr;
    }

    int32_t SockAddress::Port() const {
        return int32_t();
    }

    const sockaddr * SockAddress::RawAddr() const {
        return nullptr;
    }

    struct Socket::Implement {
        SockAddress& _LocalAddress() {
            return localAddr;
        }

        SockAddress& _PeerAddress() {
            return peerAddr;
        }

        SockAddress localAddr;
        SockAddress peerAddr;
    };

    Socket::Socket() {
    }

    Socket::~Socket() {
    }

    int32_t Socket::Open(SockFamily family, SockType type, SockProtocol protocol) {
        return int32_t();
    }

    int32_t Socket::Shutdown(SockShutdown shutdown) {
        return int32_t();
    }

    int32_t Socket::Close() {
        return int32_t();
    }

    int32_t Socket::Bind(const SockAddress & address) {
        return int32_t();
    }

    int32_t Socket::Listen(int32_t backlog) {
        return int32_t();
    }

    int32_t Socket::Connect(const SockAddress & address, const TimeSpec &timeout) {
        return int32_t();
    }

    SharedPtr<Socket> Socket::Accept(const TimeSpec &timeout) {
        return SharedPtr<Socket>();
    }

    uint32_t Socket::Select(uint32_t events, const TimeSpec &timeout) {
        return uint32_t();
    }

    int32_t Socket::Send(const void * pData, int32_t dataLen, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::SendAll(const void * pData, int32_t dataLen, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::SendTo(const void * pData, int32_t dataLen, const SockAddress & dstAddr, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::SendAllTo(const void * pData, int32_t dataLen, const SockAddress & dstAddr, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::Recv(void * pData, int32_t dataLen, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::RecvAll(void * pData, int32_t dataLen, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::RecvFrom(void * pData, int32_t dataLen, SockAddress & srcAddr, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::RecvAllFrom(void * pData, int32_t dataLen, SockAddress & srcAddr, const TimeSpec &timeout) {
        return int32_t();
    }

    int32_t Socket::SetNonblock(bool nonblock) {
        return int32_t();
    }

    int32_t Socket::IsNonblock() const {
        return int32_t();
    }

    int32_t Socket::SetOption() {
        return int32_t();
    }

    int32_t Socket::GetOption() const {
        return int32_t();
    }

    int32_t Socket::IoCtrl() {
        return int32_t();
    }

    int32_t Socket::RawFd() const {
        return int32_t();
    }

    const SockAddress & Socket::LocalAddress() const {
        return m_pImpl->_LocalAddress();
    }

    const SockAddress & Socket::PeerAddress() const {
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

    int32_t SockSelector::AddSocket(Socket * pSock, uint32_t events, SockEventHandler evtHandler, void * pData) {
        return int32_t();
    }

    int32_t SockSelector::RemoveSocket(const Socket * pSock) {
        return int32_t();
    }

    int32_t SockSelector::Select(uint32_t events, const TimeSpec &timeout) {
        return int32_t();
    }
}
