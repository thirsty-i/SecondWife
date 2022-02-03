#pragma once

#include "platform.h"

#if (PLATFORM == WINDOWS)
# if defined(_WINSOCKAPI_) && !defined(_WINSOCK2API_)
#  error WinSock.h has already been included
# endif // defined(_WINSOCKAPI_) && !defined(_WINSOCK2API_)

# include <winsock2.h>
# include <ws2tcpip.h>
# pragma comment(lib, "ws2_32.lib")

#elif (PLATFORM == LINUX) // Linux
# include <sys/socket.h> 
# include <sys/types.h>
# include<netinet/in.h>
#endif

#include <cstdint>

namespace net {
#if (PLATFORM == WINDOWS)
		typedef SOCKET socket_type;
		const SOCKET invalid_socket = INVALID_SOCKET;
		const int socket_error_retval = SOCKET_ERROR;
		const int max_addr_v4_str_len = 256;
		const int max_addr_v6_str_len = 256;
		typedef int socklen_t;
		typedef u_long ioctl_arg_type;
#elif (PLATFORM == LINUX) // Linux
		typedef int socket_type;
		const int invalid_socket = -1;
		const int socket_error_retval = -1;
		const int max_addr_v4_str_len = INET_ADDRSTRLEN;
		const int max_addr_v6_str_len = 256;
		typedef int ioctl_arg_type;
#endif
		typedef in_addr in4_addr_type;
		typedef in6_addr in6_addr_type;
		typedef sockaddr_in sockaddr_in4_type;
		typedef sockaddr_in6 sockaddr_in6_type;
		typedef sockaddr socket_addr_type;
		typedef u_short u_short_type;
} // namespace net