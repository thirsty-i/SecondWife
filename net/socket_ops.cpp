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

	socket_type accept(socket_type s, socket_addr_type* addr, socklen_t* addrlen)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::accept(s, addr, addrlen);
	}

	int bind(socket_type s, const socket_addr_type* addr, socklen_t addrlen)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::bind(s, addr, addrlen);
	}

	int listen(socket_type s, int backlog)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::listen(s, backlog);
	}

	int connect(socket_type s, const socket_addr_type* addr, socklen_t addrlen)
	{
		LOG_PROCESS_ERROR_RET(s != invalid_socket, socket_error_retval);
		return ::connect(s, addr, addrlen);
	}

	int set_non_block(socket_type s, bool value)
	{
#if (PLATFORM == WINDOWS)
		ioctl_arg_type arg = (value ? 1 : 0);
		int result = ::ioctlsocket(s, FIONBIO, &arg);
#elif (PLATFORM == LINUX)
		ioctl_arg_type arg = (value ? 1 : 0);
		int result = ::ioctl(s, FIONBIO, &arg);
#endif
		return result;
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

	signed_size_type send(socket_type s, buf* bufs, size_t count, int flags)
	{
#if (PLATFORM == WINDOWS)
		// Send the data.
		DWORD send_buf_count = static_cast<DWORD>(count);
		DWORD bytes_transferred = 0;
		DWORD send_flags = flags;
		int result = ::WSASend(s, bufs,
			send_buf_count, &bytes_transferred, send_flags, 0, 0);

		if (result != 0)
			return socket_error_retval;
		return bytes_transferred;
#elif (PLATFORM == LINUX) // Linux
		msghdr msg = msghdr();
		msg.msg_iov = const_cast<buf*>(bufs);
		msg.msg_iovlen = static_cast<int>(count);
		signed_size_type result = ::sendmsg(s, &msg, flags);
		return result;
#endif
	}

	signed_size_type recv(socket_type s, buf* bufs, size_t count, int flags)
	{
#if (PLATFORM == WINDOWS)
		// Receive some data.
		DWORD recv_buf_count = static_cast<DWORD>(count);
		DWORD bytes_transferred = 0;
		DWORD recv_flags = flags;
		int result = ::WSARecv(s, bufs, recv_buf_count,
			&bytes_transferred, &recv_flags, 0, 0);

		if (errno == WSAEMSGSIZE || errno == ERROR_MORE_DATA)
			result = 0;
		if (result != 0)
			return socket_error_retval;
		return bytes_transferred;
#elif (PLATFORM == LINUX) // Linux
		msghdr msg = msghdr();
		msg.msg_iov = bufs;
		msg.msg_iovlen = static_cast<int>(count);
		signed_size_type result = ::recvmsg(s, &msg, flags);
		return result;
#endif
	}

} // namespace net
} // namesoace socket_ops