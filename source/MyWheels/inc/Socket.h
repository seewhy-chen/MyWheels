#ifndef __MWL_SOCKET_H__
#define __MWL_SOCKET_H__

#include "inc/BasicDefines.h"
#include "inc/TimeSpan.h"
#include "inc/String.h"

#ifdef __MWL_WIN__
    #include <WinSock2.h>
    #include <ws2tcpip.h> // for socklen_t
    typedef SOCKET SockHandle;
#elif defined __MWL_LINUX__
    #include <sys/socket.h> // for sockaddr
    typedef int32_t SockHandle;
#endif

namespace mwl {

    enum SockAddressFamily {
        SOCK_AF_INVALID     = -1L,
        SOCK_AF_UNSPEC      = 0L,
        SOCK_AF_INET        = 1L,
        SOCK_AF_INET6       = 2L,
        SOCK_AF_LOCAL       = 3L,
        SOCK_AF_ABSTRACT    = 4L,

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

    enum SockEvent {
        SOCK_EVT_READ       = 1L,
        SOCK_EVT_WRITE      = 2L,
        SOCK_EVT_EXCEPT     = 4L,
    };

    class MWL_API SockAddress {
    public:
        explicit SockAddress(const String &host = nullptr, const String &service = nullptr, SockAddressFamily af = SOCK_AF_UNSPEC);
        explicit SockAddress(const String &host, int32_t port, SockAddressFamily af = SOCK_AF_UNSPEC);
        explicit SockAddress(const sockaddr *pSockAddr, socklen_t addrLen);
        SockAddress(const SockAddress &src);
        ~SockAddress();
        SockAddress &operator=(const SockAddress &rhs);

        int32_t SetAddress(const String &host, const String &service = nullptr, SockAddressFamily af = SOCK_AF_UNSPEC);
        int32_t SetAddress(const String &host, int32_t port, SockAddressFamily af = SOCK_AF_UNSPEC);
        int32_t SetAddress(const sockaddr *pSockAddr, socklen_t addrLen);

        int32_t SetHost(const String &host);
        int32_t SetHost(const sockaddr *pSockAddr, socklen_t addrLen);

        int32_t SetPort(int32_t port);
        int32_t SetPort(const sockaddr *pSockAddr, socklen_t addrLen);

        int32_t SetFamily(SockAddressFamily af);
        int32_t SetFamily(const sockaddr *pSockAddr, socklen_t addrLen);

        int32_t Resolve();
        void Reset();

        const String &Host() const;
        int32_t Port() const;
        SockAddressFamily Family() const;
        const char* FamilyName() const;
        const sockaddr *SockAddr() const;
        socklen_t SockAddrLen() const;

        void Swap(SockAddress &other);

    private:
        struct Implement;
        Implement *m_pImpl;

    };

    class MWL_API Socket : NonCopyable {
    public:
        Socket();
        Socket(SockAddressFamily af, SockType type, SockProtocol protocol = SOCK_PROTO_DEFAULT);
        ~Socket();

        int32_t Open(SockAddressFamily af, SockType type, SockProtocol protocol = SOCK_PROTO_DEFAULT);
        int32_t Reopen();
        int32_t Shutdown(SockShutdown how);
        int32_t Close();

        int32_t Bind(const SockAddress &address);
        int32_t Bind(const String &host, int32_t port = 0);
        int32_t Bind(const String &host, const String &service);
        int32_t Listen(int32_t backlog = 1);
        int32_t Connect(const SockAddress &address, const TimeSpan *pTimeout = nullptr);
        int32_t Connect(const String &host, int32_t port = 0, const TimeSpan *pTimeout = nullptr);
        int32_t Connect(const String &host, const String &service, const TimeSpan *pTimeout = nullptr);
        int32_t Accept(Socket &acceptee, const TimeSpan *pTimeout = nullptr);
        int32_t Select(uint32_t events, const TimeSpan *pTimeout = nullptr);

        int32_t Send(const void *pData, size_t dataLen, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);
        int32_t SendAll(const void *pData, size_t dataLen, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);
        int32_t SendTo(const void *pData, size_t dataLen, const SockAddress &dstAddr, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);
        int32_t SendAllTo(const void *pData, size_t dataLen, const SockAddress &dstAddr, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);

        int32_t Recv(void *pData, size_t dataLen, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);
        int32_t RecvAll(void *pData, size_t dataLen, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);
        int32_t RecvFrom(void *pData, size_t dataLen, SockAddress &srcAddr, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);
        int32_t RecvAllFrom(void *pData, size_t dataLen, SockAddress &srcAddr, const TimeSpan *pTimeout = nullptr, int32_t flags = 0);

        int32_t SetNonblocking(bool nonblocking);
        bool IsNonblocking() const;

        int32_t SetOption(int32_t level, int32_t optName, const void *pOptVal, socklen_t valLen);
        template<typename OptionType>
        int32_t SetOption(int32_t level, int32_t optName, const OptionType &option) {
            return SetOption(level, optName, &option, sizeof(option));
        }

        int32_t GetOption(int32_t level, int32_t optName, void *pOptVal, socklen_t *valLen) const;
        template<typename OptionType>
        int32_t GetOption(int32_t level, int32_t optName, OptionType &option) {
            socklen_t optLen = sizeof(option);
            return GetOption(level, optName, &option, &optLen);
        }

        const SockAddress &LocalAddress() const;
        const SockAddress &PeerAddress() const;

    private:
        struct Implement;
        Implement *m_pImpl;
    };

}

#endif // __MWL_SOCKET_H__
