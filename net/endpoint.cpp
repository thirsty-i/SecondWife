#include "platform.h"
#include "endpoint.h"
#include "socket_ops.h"

namespace net {
	endpoint::endpoint() noexcept
		: data_()
	{
		data_.v4.sin_family = AF_INET;
		data_.v4.sin_port = 0;
		data_.v4.sin_addr.s_addr = INADDR_ANY;
	}

	endpoint::endpoint(int family, unsigned short port_num) noexcept
		: data_()
	{
		if (family == (AF_INET))
		{
			data_.v4.sin_family = (AF_INET);
			data_.v4.sin_port =
				socket_ops::host_to_network_short(port_num);
			data_.v4.sin_addr.s_addr = (INADDR_ANY);
		}
		else
		{
			data_.v6.sin6_family = (AF_INET6);
			data_.v6.sin6_port =
				socket_ops::host_to_network_short(port_num);
			data_.v6.sin6_flowinfo = 0;
			data_.v6.sin6_addr.s6_addr[0] = 0; data_.v6.sin6_addr.s6_addr[1] = 0;
			data_.v6.sin6_addr.s6_addr[2] = 0; data_.v6.sin6_addr.s6_addr[3] = 0;
			data_.v6.sin6_addr.s6_addr[4] = 0; data_.v6.sin6_addr.s6_addr[5] = 0;
			data_.v6.sin6_addr.s6_addr[6] = 0; data_.v6.sin6_addr.s6_addr[7] = 0;
			data_.v6.sin6_addr.s6_addr[8] = 0; data_.v6.sin6_addr.s6_addr[9] = 0;
			data_.v6.sin6_addr.s6_addr[10] = 0; data_.v6.sin6_addr.s6_addr[11] = 0;
			data_.v6.sin6_addr.s6_addr[12] = 0; data_.v6.sin6_addr.s6_addr[13] = 0;
			data_.v6.sin6_addr.s6_addr[14] = 0; data_.v6.sin6_addr.s6_addr[15] = 0;
			data_.v6.sin6_scope_id = 0;
		}
	}

	endpoint::endpoint(const ip::address& addr,
		unsigned short port_num) noexcept
		: data_()
	{
		using namespace std; // For memcpy.
		if (addr.is_v4())
		{
			data_.v4.sin_family = (AF_INET);
			data_.v4.sin_port =
				socket_ops::host_to_network_short(port_num);
			data_.v4.sin_addr.s_addr =
				socket_ops::host_to_network_long(
					addr.to_v4().to_uint());
		}
		else
		{
			data_.v6.sin6_family = (AF_INET6);
			data_.v6.sin6_port =
				socket_ops::host_to_network_short(port_num);
			data_.v6.sin6_flowinfo = 0;
			ip::address_v6 v6_addr = addr.to_v6();
			ip::address_v6::bytes_type bytes = v6_addr.to_bytes();
			memcpy(data_.v6.sin6_addr.s6_addr, bytes.data(), 16);
			data_.v6.sin6_scope_id =
				static_cast<u_long>(
					v6_addr.scope_id());
		}
	}

	unsigned short endpoint::port() const noexcept
	{
		if (is_v4())
		{
			return socket_ops::network_to_host_short(
				data_.v4.sin_port);
		}
		else
		{
			return socket_ops::network_to_host_short(
				data_.v6.sin6_port);
		}
	}

	void endpoint::port(unsigned short port_num) noexcept
	{
		if (is_v4())
		{
			data_.v4.sin_port
				= socket_ops::host_to_network_short(port_num);
		}
		else
		{
			data_.v6.sin6_port
				= socket_ops::host_to_network_short(port_num);
		}
	}

	ip::address endpoint::address() const noexcept
	{
		using namespace std; // For memcpy.
		if (is_v4())
		{
			return ip::address_v4(
				socket_ops::network_to_host_long(
					data_.v4.sin_addr.s_addr));
		}
		else
		{
			ip::address_v6::bytes_type bytes;
			memcpy(bytes.data(), data_.v6.sin6_addr.s6_addr, 16);
			return ip::address_v6(bytes, data_.v6.sin6_scope_id);
		}
	}

	void endpoint::address(const ip::address& addr) noexcept
	{
		endpoint tmp_endpoint(addr, port());
		data_ = tmp_endpoint.data_;
	}

	bool operator==(const endpoint& e1, const endpoint& e2) noexcept
	{
		return e1.address() == e2.address() && e1.port() == e2.port();
	}

	bool operator<(const endpoint& e1, const endpoint& e2) noexcept
	{
		if (e1.address() < e2.address())
			return true;
		if (e1.address() != e2.address())
			return false;
		return e1.port() < e2.port();
	}

	std::string endpoint::to_string() const
	{
		std::ostringstream tmp_os;
		tmp_os.imbue(std::locale::classic());
		if (is_v4())
			tmp_os << address().to_string();
		else
			tmp_os << '[' << address().to_string() << ']';
		tmp_os << ':' << port();

		return tmp_os.str();
	}

	

} // namespace net

