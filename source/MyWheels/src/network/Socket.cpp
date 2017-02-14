#include "inc/Socket.h"
#include "SockAddressImplement.h"

namespace mwl {

    struct Socket::Implement {
        SockAddress &_LocalAddress() {
            return localAddr;
        }

        SockAddress &_PeerAddress() {
            return peerAddr;
        }

        SockAddress localAddr;
        SockAddress peerAddr;
    };

    Socket::Socket() {
    }

    Socket::~Socket() {
    }

    int32_t Socket::Open(SockAddressFamily family, SockType type, SockProtocol protocol) {
        return -1;
    }

    int32_t Socket::Shutdown(SockShutdown shutdown) {
        return -1;
    }

    int32_t Socket::Close() {
        return -1;
    }

    int32_t Socket::Bind(const SockAddress &address) {
        return -1;
    }

    int32_t Socket::Listen(int32_t backlog) {
        return -1;
    }

    int32_t Socket::Connect(const SockAddress &address, const TimeSpec &timeout) {
        return -1;
    }

    SharedPtr<Socket> Socket::Accept(const TimeSpec &timeout) {
        return SharedPtr<Socket>();
    }

    uint32_t Socket::Select(uint32_t events, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::Send(const void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::SendAll(const void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::SendTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::SendAllTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::Recv(void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::RecvAll(void *pData, int32_t dataLen, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::RecvFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::RecvAllFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec &timeout) {
        return -1;
    }

    int32_t Socket::SetNonblock(bool nonblock) {
        return -1;
    }

    int32_t Socket::IsNonblock() const {
        return -1;
    }

    int32_t Socket::SetOption() {
        return -1;
    }

    int32_t Socket::GetOption() const {
        return -1;
    }

    int32_t Socket::IoCtrl() {
        return -1;
    }

    int32_t Socket::RawFd() const {
        return -1;
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
