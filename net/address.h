# pragma once

#include "address_v4.h"
#include "address_v6.h"
#include <string>

namespace net {
class address
{
public:
    address() noexcept;

    address(
        const address_v4& ipv4_address) noexcept;

    /// Construct an address from an IPv6 address.
    address(
        const address_v6& ipv6_address) noexcept;

    address(const address& other) noexcept;

    address(address&& other) noexcept;

    address& operator=(const address& other) noexcept;

    address& operator=(address&& other) noexcept;

    address& operator=(
        const address_v4& ipv4_address) noexcept;

    address& operator=(
        const address_v6& ipv6_address) noexcept;

    bool is_v4() const noexcept
    {
        return type_ == ipv4;
    }

    bool is_v6() const noexcept
    {
        return type_ == ipv6;
    }

    address_v4 to_v4() const;

    address_v6 to_v6() const;

    std::string to_string() const;

    static address from_string(const char* str);

    static address from_string(const std::string& str);

    bool is_loopback() const noexcept;

    bool is_unspecified() const noexcept;

    bool is_multicast() const noexcept;

    friend bool operator==(const address& a1,
        const address& a2) noexcept;

    friend bool operator!=(const address& a1,
        const address& a2) noexcept
    {
        return !(a1 == a2);
    }

    friend bool operator<(const address& a1,
        const address& a2) noexcept;

    friend bool operator>(const address& a1,
        const address& a2) noexcept
    {
        return a2 < a1;
    }

    friend bool operator<=(const address& a1,
        const address& a2) noexcept
    {
        return !(a2 < a1);
    }

    friend bool operator>=(const address& a1,
        const address& a2) noexcept
    {
        return !(a1 < a2);
    }

private:
    enum { ipv4, ipv6 } type_;

    address_v4 ipv4_address_;

    address_v6 ipv6_address_;
};

address make_address(const char* str);

address make_address(const std::string& str);
} // namespace net
