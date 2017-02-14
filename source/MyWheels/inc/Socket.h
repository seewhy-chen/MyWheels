#ifndef __MWL_SOCKET_H__
#define __MWL_SOCKET_H__

#include "inc/BasicDefines.h"
#include "inc/TimeDefines.h"
#include "inc/SmartPointer.h"

#ifdef __MWL_WIN__
    #include <WinSock2.h>
#elif defined __MWL_LINUX__
    #include <sys/socket.h> // for sockaddr
#endif

namespace mwl {

    enum SockAddressFamily {
        SOCK_AF_INVALID = -1L,
        SOCK_AF_UNSPEC = 0L,
        SOCK_AF_INET    = 1L,
        SOCK_AF_INET6   = 2L,
        SOCK_AF_LOCAL   = 3L,

        SockAddressFamilyCount
    };

    enum SockType {
        SOCK_TYPE_INVALID   = -1L,
        SOCK_TYPE_STREAM    = 0L,
        SOCK_TYPE_DGRAM     = 1L,
        SOCK_TYPE_SEQPKT    = 2L,
        SOCK_TYPE_RAW       = 3L,

        SockTypeCount
    };

    enum SockProtocol {
        SOCK_PROTO_INVALID  = -1L,
        SOCK_PROTO_DEFAULT  = 0L,
        SOCK_PROTO_TCP      = 1L,
        SOCK_PROTO_UDP      = 2L,
        SOCK_PROTO_SCTP     = 3L,

        SockProtocolCount
    };

    enum SockShutdown {
        SOCK_SHUT_RD        = 0L,
        SOCK_SHUT_WR        = 1L,
        SOCK_SHUT_RDWR      = 2L,

        SockShutdownCount
    };

    class MWL_API SockAddress {
    public:
        explicit SockAddress(const char *host = nullptr, const char *service = nullptr, SockAddressFamily af = SOCK_AF_UNSPEC);
        explicit SockAddress(const char *host, int32_t port, SockAddressFamily af = SOCK_AF_UNSPEC);
        explicit SockAddress(const sockaddr *pSockAddr);
        SockAddress(const SockAddress &src);
        ~SockAddress();
        SockAddress &operator=(const SockAddress &rhs);

        // address: host + port
        int32_t SetAddress(const char *host, const char *service, SockAddressFamily af = SOCK_AF_UNSPEC);
        int32_t SetAddress(const char *host, int32_t port, SockAddressFamily af = SOCK_AF_UNSPEC);
        int32_t SetAddress(const sockaddr *pSockAddr);

        int32_t SetHost(const char *host);
        int32_t SetHost(const sockaddr *pSockAddr);

        int32_t SetPort(int32_t port);
        int32_t SetPort(const sockaddr *pSockAddr);

        int32_t SetFamily(SockAddressFamily af);
        int32_t SetFamily(const sockaddr *pSockAddr);

        int32_t Resolve();

        const char *Host() const;
        int32_t Port() const;
        SockAddressFamily AddressFamily() const;
        const sockaddr* RawAddr() const;

        void Swap(SockAddress &other);

        struct Implement;
        Implement *Impl();

    private:
        Implement *m_pImpl;

    };

    class MWL_API Socket : private NonCopyable {
    public:
        Socket();
        ~Socket();

        int32_t Open(SockAddressFamily family, SockType type, SockProtocol protocol);
        int32_t Shutdown(SockShutdown shutdown);
        int32_t Close();

        int32_t Bind(const SockAddress &address);
        int32_t Listen(int32_t backlog);
        int32_t Connect(const SockAddress &address, const TimeSpec &timeout = MWL_PERMANANT);
        SharedPtr<Socket> Accept(const TimeSpec &timeout = MWL_PERMANANT);
        uint32_t Select(uint32_t events, const TimeSpec &timeout = MWL_PERMANANT);

        int32_t Send(const void *pData, int32_t dataLen, const TimeSpec &timeout = MWL_PERMANANT);
        int32_t SendAll(const void *pData, int32_t dataLen, const TimeSpec &timeout = MWL_PERMANANT);
        int32_t SendTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec &timeout = MWL_PERMANANT);
        int32_t SendAllTo(const void *pData, int32_t dataLen, const SockAddress &dstAddr, const TimeSpec &timeout = MWL_PERMANANT);

        int32_t Recv(void *pData, int32_t dataLen, const TimeSpec &timeout = MWL_PERMANANT);
        int32_t RecvAll(void *pData, int32_t dataLen, const TimeSpec &timeout = MWL_PERMANANT);
        int32_t RecvFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec &timeout = MWL_PERMANANT);
        int32_t RecvAllFrom(void *pData, int32_t dataLen, SockAddress &srcAddr, const TimeSpec &timeout = MWL_PERMANANT);

        int32_t SetNonblock(bool nonblock);
        int32_t IsNonblock() const;
        int32_t SetOption();
        int32_t GetOption() const;
        int32_t IoCtrl();

        int32_t RawFd() const;
        const SockAddress &LocalAddress() const;
        const SockAddress &PeerAddress() const;

    private:
        struct Implement;
        Implement *m_pImpl;
    };

    typedef void (*SockEventHandler)(Socket *pSock, uint32_t event, void *pData);
    class MWL_API SockSelector : private NonCopyable {
    public:
        SockSelector();
        ~SockSelector();
        int32_t AddSocket(Socket *pSock, uint32_t events, SockEventHandler evtHandler, void *pData);
        int32_t RemoveSocket(const Socket *pSock);
        int32_t Select(uint32_t events, const TimeSpec &timeout = MWL_PERMANANT);
    };

    void SockGetHostByName();
    void SockGetHostByAddr();
    void SockGetServByName();
    void SockGetAddrInfo();
}

#endif // __MWL_SOCKET_H__
