#include "address_v4.h"
#include "socket_ops.h"
#include "socket_types.h"
#include "logger/log.h"

namespace net {
namespace ip {
address_v4::address_v4(const address_v4::bytes_type& bytes)
{
#if UCHAR_MAX > 0xFF
	if (bytes[0] > 0xFF || bytes[1] > 0xFF
		|| bytes[2] > 0xFF || bytes[3] > 0xFF)
	{
		std::out_of_range ex("address_v4 from bytes_type");
		asio::detail::throw_exception(ex);
	}
#endif // UCHAR_MAX > 0xFF

	using namespace std; // For memcpy.
	memcpy(&addr_.s_addr, bytes.data(), 4);
}

address_v4::address_v4(address_v4::uint_type addr)
{
	LOG_PROCESS_ERROR((std::numeric_limits<uint_type>::max)() <= 0xFFFFFFFF);

	addr_.s_addr = socket_ops::host_to_network_long(
		static_cast<u_long>(addr));
}

address_v4::bytes_type address_v4::to_bytes() const noexcept
{
	using namespace std; // For memcpy.
	bytes_type bytes;
	memcpy(bytes.data(), &addr_.s_addr, 4);
	return bytes;
}

address_v4::uint_type address_v4::to_uint() const noexcept
{
	return socket_ops::network_to_host_long(addr_.s_addr);
}

unsigned long address_v4::to_ulong() const
{
	return socket_ops::network_to_host_long(addr_.s_addr);
}

std::string address_v4::to_string() const
{
	char addr_str[max_addr_v4_str_len];
	const char* addr =
		socket_ops::inet_ntop(
			(AF_INET), &addr_, addr_str,
			max_addr_v4_str_len, 0);
	if (addr == 0)
		return std::string();
	return addr;
}

inline address_v4 address_v4::from_string(const char* str)
{
	return make_address_v4(str);
}

inline address_v4 address_v4::from_string(const std::string& str)
{
	return make_address_v4(str);
}

bool address_v4::is_loopback() const noexcept
{
	return (to_uint() & 0xFF000000) == 0x7F000000;
}

bool address_v4::is_unspecified() const noexcept
{
	return to_uint() == 0;
}

bool address_v4::is_multicast() const noexcept
{
	return (to_uint() & 0xF0000000) == 0xE0000000;
}

address_v4 make_address_v4(const char* str) noexcept
{
	address_v4::bytes_type bytes;
	if (socket_ops::inet_pton(
		(AF_INET), str, &bytes, 0) <= 0)
		return address_v4();
	return address_v4(bytes);
}

address_v4 make_address_v4(const std::string& str) noexcept
{
	return make_address_v4(str.c_str());
}
} // namespace ip
} // namespace net


