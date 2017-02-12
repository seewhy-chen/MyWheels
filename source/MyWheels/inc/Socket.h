#ifndef __MWL_SOCKET_H__
#define __MWL_SOCKET_H__

#include "inc/BasicDefines.h"
#include "inc/TimeDefines.h"
#include "inc/SmartPointer.h"

namespace mwl {

    enum SockFamily {

    };

    enum SockType {

    };

    enum SockProtocol {

    };

    enum SockShutdown {

    };

    class MWL_API SockAddress {
    public:
        explicit SockAddress(const char *strAddr = nullptr);
        explicit SockAddress(const sockaddr* pSockAddr);
        SockAddress(const SockAddress &src);
        ~SockAddress();
        SockAddress& operator=(const SockAddress &rhs);

        int32_t SetAddress(const char *strAddr);
        int32_t SetAddress(const sockaddr* pSockAddr);
        int32_t SetHost(const char *strHost);
        int32_t SetHost(const sockaddr* pSockAddr);
        int32_t SetPort(int32_t port);
        int32_t SetPort(const sockaddr* pSockAddr);

        const char *Address() const;
        const char *Host() const;
        int32_t Port() const;
        const sockaddr* RawAddr() const;

        bool IsSameHost(const char *strHost) const;
        bool IsSamePort(int32_t port) const;
        bool IsSamePort(const SockAddress &rhs) const;
        bool operator==(const SockAddress &rhs) const;
        void Swap(SockAddress &other);
        
        struct Implement;
        Implement* Impl();

    private:
        Implement *m_pImpl;

    };

    class MWL_API Socket : private NonCopyable {
    public:
        Socket();
        ~Socket();

        int32_t Open(SockFamily family, SockType type, SockProtocol protocol);
        int32_t Shutdown(SockShutdown shutdown);
        int32_t Close();

        int32_t Bind(const SockAddress &address);
        int32_t Listen(int32_t backlog);
        int32_t Connect(const SockAddress &address, int64_t timeout = -1, TimeUnit unit = MILLISEC);
        SharedPtr<Socket> Accept(int64_t timeout = -1, TimeUnit unit = MILLISEC);
        uint32_t Select(uint32_t events, int64_t timeout = -1, TimeUnit unit = MILLISEC);

        int32_t Send(const void *pData, int32_t dataLen, int64_t timeout = -1, TimeUnit unit = MILLISEC);
        int32_t SendAll(const void *pData, int32_t dataLen, int64_t timeout = -1, TimeUnit unit = MILLISEC);
        int32_t SendTo(const void *pData, int32_t dataLen, const SockAddress& dstAddr, int64_t timeout = -1, TimeUnit unit = MILLISEC);
        int32_t SendAllTo(const void *pData, int32_t dataLen, const SockAddress& dstAddr, int64_t timeout = -1, TimeUnit unit = MILLISEC);

        int32_t Recv(void *pData, int32_t dataLen, int64_t timeout = -1, TimeUnit unit = MILLISEC);
        int32_t RecvAll(void *pData, int32_t dataLen, int64_t timeout = -1, TimeUnit unit = MILLISEC);
        int32_t RecvFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, int64_t timeout = -1, TimeUnit unit = MILLISEC);
        int32_t RecvAllFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, int64_t timeout = -1, TimeUnit unit = MILLISEC);

        int32_t SetNonblock(bool nonblock);
        int32_t IsNonblock() const;
        int32_t SetOption();
        int32_t GetOption() const;
        int32_t IoCtrl();

        int32_t RawFd() const;
        const SockAddress& LocalAddress() const;
        const SockAddress& PeerAddress() const;

    private:
        struct Implement;
        Implement *m_pImpl;
    };

    typedef void (*SockEventHandler)(Socket* pSock, uint32_t event, void *pData);
    class MWL_API SockSelector : private NonCopyable {
    public:
        SockSelector();
        ~SockSelector();
        int32_t AddSocket(Socket* pSock, uint32_t events, SockEventHandler evtHandler, void *pData);
        int32_t RemoveSocket(const Socket* pSock);
        int32_t Select(uint32_t events, int64_t timeout = -1, TimeUnit unit = MILLISEC);
    };

    void SockGetHostByName();
    void SockGetHostByAddr();
    void SockGetServByName();
    void SockGetAddrInfo();
}

#endif // __MWL_SOCKET_H__
