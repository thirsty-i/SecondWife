#pragma once

#include "socket_types.h"
#include "address.h"
#include <string>

namespace net {
class endpoint
{
public:
	endpoint() noexcept;

	endpoint(int family,
		unsigned short port_num) noexcept;

	endpoint(const address& addr,
		unsigned short port_num) noexcept;

	endpoint(const endpoint& other) noexcept
		: data_(other.data_)
	{
	}

	endpoint& operator=(const endpoint& other) noexcept
	{
		data_ = other.data_;
		return *this;
	}

	socket_addr_type* data() noexcept
	{
		return &data_.base;
	}

	const socket_addr_type* data() const noexcept
	{
		return &data_.base;
	}

	socklen_t size() const noexcept
	{
		if (is_v4())
			return sizeof(sockaddr_in4_type);
		else
			return sizeof(sockaddr_in6_type);
	}

	size_t capacity() const noexcept
	{
		return sizeof(data_);
	}

	unsigned short port() const noexcept;

	void port(unsigned short port_num) noexcept;

	address address() const noexcept;

	void address(
		const address& addr) noexcept;

	friend bool operator==(const endpoint& e1,
		const endpoint& e2) noexcept;

	friend bool operator<(const endpoint& e1,
		const endpoint& e2) noexcept;

	bool is_v4() const noexcept
	{
		return data_.base.sa_family == AF_INET;
	}

	std::string to_string() const;

private:
	union data_union
	{
		socket_addr_type base;
		sockaddr_in4_type v4;
		sockaddr_in6_type v6;
	} data_;
};
}; // namespace net