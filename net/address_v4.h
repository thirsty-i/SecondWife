# pragma once

#include <cstdint>
#include <array>
#include <string>
#include "socket_types.h"

namespace net {
class address_v4
{
public:
    typedef uint32_t uint_type;

    typedef std::array<unsigned char, 4> bytes_type;

    address_v4() noexcept
    {
        addr_.s_addr = 0;
    }

    explicit address_v4(const bytes_type& bytes);

    explicit address_v4(uint_type addr);

    address_v4(const address_v4& other) noexcept
        : addr_(other.addr_)
    {
    }

    address_v4(address_v4&& other) noexcept
        : addr_(other.addr_)
    {
    }

    address_v4& operator=(const address_v4& other) noexcept
    {
        addr_ = other.addr_;
        return *this;
    }

    address_v4& operator=(address_v4&& other) noexcept
    {
        addr_ = other.addr_;
        return *this;
    }

    bytes_type to_bytes() const noexcept;

    uint_type to_uint() const noexcept;

    unsigned long to_ulong() const;

    std::string to_string() const;

    static address_v4 from_string(const char* str);

    static address_v4 from_string(const std::string& str);

    bool is_loopback() const noexcept;
    bool is_unspecified() const noexcept;

    bool is_class_a() const;
    bool is_class_b() const;
    bool is_class_c() const;

    bool is_multicast() const noexcept;

    /// Compare two addresses for equality.
    friend bool operator==(const address_v4& a1,
        const address_v4& a2) noexcept
    {
        return a1.addr_.s_addr == a2.addr_.s_addr;
    }

    friend bool operator!=(const address_v4& a1,
        const address_v4& a2) noexcept
    {
        return a1.addr_.s_addr != a2.addr_.s_addr;
    }

    friend bool operator<(const address_v4& a1,
        const address_v4& a2) noexcept
    {
        return a1.to_uint() < a2.to_uint();
    }

    friend bool operator>(const address_v4& a1,
        const address_v4& a2) noexcept
    {
        return a1.to_uint() > a2.to_uint();
    }

    friend bool operator<=(const address_v4& a1,
        const address_v4& a2) noexcept
    {
        return a1.to_uint() <= a2.to_uint();
    }

    friend bool operator>=(const address_v4& a1,
        const address_v4& a2) noexcept
    {
        return a1.to_uint() >= a2.to_uint();
    }

    static address_v4 any() noexcept
    {
        return address_v4();
    }

    static address_v4 loopback() noexcept
    {
        return address_v4(0x7F000001);
    }

    static address_v4 broadcast() noexcept
    {
        return address_v4(0xFFFFFFFF);
    }

    static address_v4 broadcast(
        const address_v4& addr, const address_v4& mask);

    static address_v4 netmask(const address_v4& addr);

private:
    in4_addr_type addr_;
};

inline address_v4 make_address_v4(const address_v4::bytes_type& bytes)
{
    return address_v4(bytes);
}

inline address_v4 make_address_v4(address_v4::uint_type addr)
{
    return address_v4(addr);
}

address_v4 make_address_v4(const char* str);

address_v4 make_address_v4(const std::string& str);

} // namespace net