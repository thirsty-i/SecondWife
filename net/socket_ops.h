#pragma once
#include "socket_types.h"
#include <stddef.h>
#include "logger/log.h"


namespace net {
namespace socket_ops{
	socket_type socket(int af, int type, int protocol);

	socket_type accept(socket_type s, socket_addr_type* addr, socklen_t* addrlen);

	int bind(socket_type s, const socket_addr_type* addr, socklen_t addrlen);

	int listen(socket_type s, int backlog);

	int connect(socket_type s, const socket_addr_type* addr, socklen_t addrlen);

	int set_non_block(socket_type s, bool value);

	u_long network_to_host_long(u_long value);

	u_long host_to_network_long(u_long value);

	u_short_type network_to_host_short(u_short_type value);

	u_short_type host_to_network_short(u_short_type value);

	const char* inet_ntop(int af, const void* src, char* dest, size_t length,
		unsigned long scope_id);

	int inet_pton(int af, const char* src, void* dest, unsigned long* scope_id);

	int gethostname(char* name, int namelen);

	signed_size_type send(socket_type s, buf* bufs, size_t count, int flags);
	signed_size_type recv(socket_type s, buf* bufs, size_t count, int flags);

};
};