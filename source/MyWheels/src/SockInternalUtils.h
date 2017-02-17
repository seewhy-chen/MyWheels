#ifndef __MWL_SOCK_INTERNAL_UTILS_H__
#define __MWL_SOCK_INTERNAL_UTILS_H__

#include "inc/Socket.h"

#ifdef __MWL_WIN__
    #include <WinSock2.h>
    #include <ws2ipdef.h> // for sockaddr_in6
    #include <WS2tcpip.h> // for inet_ntop
    #include <io.h>       // for open/close
    #define sock_errno WSAGetLastError()

    #define EWOULDBLOCK             WSAEWOULDBLOCK
    #define EINPROGRESS             WSAEINPROGRESS
    #define EALREADY                WSAEALREADY
#elif defined __MWL_LINUX__
    #include <netdb.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <arpa/inet.h> // for inet_ntop
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/un.h>    // for sockaddr_un

    #ifndef SUN_LEN
        #define SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path) + strlen ((ptr)->sun_path) + 1)
    #endif

    #define closesocket close
    #define INVALID_SOCKET (-1)
    #define sock_errno errno

#endif

namespace mwl {

    static const int32_t s_afMap[] = {
        AF_UNSPEC,  // SOCK_AF_UNSPEC
        AF_INET,    // SOCK_AF_INET
        AF_INET6,   // SOCK_AF_INET6
        AF_UNIX,    // SOCK_AF_LOCAL
        AF_UNIX,    // SOCK_AF_ABSTRACT

    };
    MWL_STATIC_ASSERT(MWL_ARR_SIZE(s_afMap) == SockAddressFamilyCount, some_sock_family_is_missing);

    static const int32_t s_typeMap[] = {
        SOCK_STREAM,    // SOCK_TYPE_STREAM
        SOCK_DGRAM,     // SOCK_TYPE_DGRAM
        SOCK_SEQPACKET, // SOCK_TYPE_SEQPKT
        SOCK_RAW,       // SOCK_TYPE_RAW
    };
    MWL_STATIC_ASSERT(MWL_ARR_SIZE(s_typeMap) == SockTypeCount, some_sock_type_is_missing);

    static const int32_t s_protoMap[] = {
        0,              // SOCK_PROTO_DEFAULT
        IPPROTO_TCP,    // SOCK_PROTO_TCP
        IPPROTO_IP,     // SOCK_PROTO_UDP
        IPPROTO_SCTP,   // SOCK_PROTO_SCTP
    };
    MWL_STATIC_ASSERT(MWL_ARR_SIZE(s_protoMap) == SockProtocolCount, some_sock_proto_is_missing);

    static const int32_t s_shutdownMap[] = {
#ifdef __MWL_WIN__
        SD_RECEIVE,     // SOCK_SHUT_RD
        SD_SEND,        // SOCK_SHUT_RD
        SD_BOTH,        // SOCK_SHUT_RDWR
#else
        SHUT_RD,        // SOCK_SHUT_RD
        SHUT_WR,        // SOCK_SHUT_RD
        SHUT_RDWR,      // SOCK_SHUT_RDWR,
#endif
    };
    MWL_STATIC_ASSERT(MWL_ARR_SIZE(s_shutdownMap) == SockShutdownCount, some_sock_shut_is_missing);

    static void inline _initializeSock() {
#ifdef __MWL_WIN__
#define WS_VERSION_CHOICE1 0x202    /*MAKEWORD(2,2)*/
#define WS_VERSION_CHOICE2 0x101    /*MAKEWORD(1,1)*/
        static bool _winSockInitialized = false;
        WSADATA wsadata;
        if (!_winSockInitialized) {
            int32_t ret = WSAStartup(WS_VERSION_CHOICE1, &wsadata);
            if (ret != 0) {
                MWL_WARN("WSAStartup failed, ret = %d", ret);
                ret = WSAStartup(WS_VERSION_CHOICE2, &wsadata);
                if (ret != 0) {
                    MWL_WARN("WSAStartup2 failed, ret = %d", ret);
                }
                return;
            }
            if ((wsadata.wVersion != WS_VERSION_CHOICE1)
                    && (wsadata.wVersion != WS_VERSION_CHOICE2)) {
                WSACleanup();
                MWL_WARN("invalid winsock version: 0x%x(%d)", wsadata.wVersion, wsadata.wVersion);
            } else {
                _winSockInitialized = true;
            }
        }
#endif
    }

}

#endif // __MWL_SOCK_INTERNAL_UTILS_H__
