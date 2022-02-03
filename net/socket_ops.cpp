#include "socket_ops.h"

#if PLATFORM == WINDOWS
#elif PLATFORM == LINUX
# include <sys/ioctl.h>
# include <arpa/inet.h>
#endif

namespace net {
namespace socket_ops {
	socket_type socket(int af, int type, int protocol)
	{
		return ::socket(af, type, protocol);
	}

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
		return true;
	}

	u_long network_to_host_long(u_long value)
	{
		return ::ntohl(value);
	}

	u_long host_to_network_long(u_long value)
	{
		return ::htonl(value);
	}

	u_short_type network_to_host_short(u_short_type value)
	{
		return ::ntohs(value);
	}

	u_short_type host_to_network_short(u_short_type value)
	{
		return ::htons(value);
	}

	const char* inet_ntop(int af, const void* src, char* dest, size_t length,
		unsigned long scope_id)
	{
		return ::inet_ntop(af, src, dest, static_cast<int>(length));
	}

	int inet_pton(int af, const char* src, void* dest, unsigned long* scope_id)
	{
		return ::inet_pton(af, src, dest);
	}

	int gethostname(char* name, int namelen)
	{
		return ::gethostname(name, namelen);
	}

} // namespace net
} // namesoace socket_ops