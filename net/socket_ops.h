#pragma once

namespace net {
namespace socket_ops{
	inline socket_type accept(socket_type s, socket_addr_type* addr,
		std::size_t* addrlen, asio::error_code& ec);


};
};