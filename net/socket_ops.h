#pragma once
#include "socket_types.h"
#include <stddef.h>
#include "logger/log.h"

namespace net {
namespace socket_ops{
	inline socket_type accept(socket_type s, socket_addr_type* addr, socklen_t* addrlen)
	{

		msghdr::msg_namelen;
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::accept(s, addr, addrlen);
	}

	inline int bind(socket_type s, const socket_addr_type* addr, socklen_t addrlen)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::bind(s, addr, addrlen);
	}

	inline int listen(socket_type s, int backlog)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::listen(s, backlog);;
	}

	inline int connect(socket_type s, const socket_addr_type* addr, socklen_t addrlen)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::connect(s, addr, addrlen);
	}

};
};