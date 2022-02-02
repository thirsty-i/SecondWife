#pragma once

#include "platform.h"

#if (PLATFORM == WINDOWS)
# include <winsock2.h>
#elif (PLATFORM == LINUX) // Linux
# include <sys/socket.h> 
# include <sys/types.h>
# include<netinet/in.h>
#endif

namespace net {
#if (PLATFORM == WINDOWS)
		typedef SOCKET socket_type;
		const SOCKET invalid_socket = INVALID_SOCKET;
		const int socket_error_retval = SOCKET_ERROR;
		const int max_addr_v4_str_len = 256;
		typedef sockaddr socket_addr_type;
		typedef in_addr in4_addr_type;
		typedef sockaddr_in sockaddr_in4_type;
		typedef int socklen_t;
#elif (PLATFORM == LINUX) // Linux
		typedef int socket_type;
		const int invalid_socket = -1;
		const int socket_error_retval = -1;
		const int max_addr_v4_str_len = INET_ADDRSTRLEN;
		typedef sockaddr socket_addr_type;
		typedef in_addr in4_addr_type;
		typedef sockaddr_in sockaddr_in4_type;
#endif
} // namespace net