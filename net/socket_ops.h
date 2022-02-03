#pragma once
#include "socket_types.h"
#include <stddef.h>
#include "logger/log.h"

#if PLATFORM == WINDOWS
#elif PLATFORM == LINUX
# include <sys/ioctl.h>
#endif


namespace net {
namespace socket_ops{
	inline socket_type accept(socket_type s, socket_addr_type* addr, socklen_t* addrlen)
	{
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
		return ::listen(s, backlog);
	}

	inline int connect(socket_type s, const socket_addr_type* addr, socklen_t addrlen)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::connect(s, addr, addrlen);
	}

	bool set_non_block(socket_type s, bool value)
	{
#if (PLATFORM == WINDOWS)
		ioctl_arg_type arg = (value ? 1 : 0);
		int result = ::ioctlsocket(s, FIONBIO, &arg);
#elif (PLATFORM == LINUX)
		ioctl_arg_type arg = (value ? 1 : 0);
		int result = ::ioctl(s, FIONBIO, &arg);
#endif
	}

	u_long network_to_host_long(u_long value)
	{
		return ntohl(value);
	}

	u_long host_to_network_long(u_long value)
	{
		return htonl(value);
	}

	u_long network_to_host_short(u_long value)
	{
		return ntohs(value);
	}

	u_long host_to_network_short(u_long value)
	{
		return htons(value);
	}

	const char* inet_ntop(int af, const void* src, char* dest, size_t length,
		unsigned long scope_id)
	{
		//TODO: implement
	}

	int inet_pton(int af, const char* src, void* dest,
		unsigned long* scope_id)
	{
		//TODO: implement
	}

	int gethostname(char* name, int namelen)
	{
		int result = ::gethostname(name, namelen);
		return ::gethostname(name, namelen);
	}

};
};