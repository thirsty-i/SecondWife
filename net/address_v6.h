# pragma once
#include "socket_types.h"
#include "address_v4.h"
#include <array>
#include <string>

namespace net {

class address_v6
{
public:
    typedef std::array<unsigned char, 16> bytes_type;

    address_v6() noexcept;

    explicit address_v6(const bytes_type& bytes,
        unsigned long scope_id = 0);

    address_v6(const address_v6& other) noexcept;

    address_v6(address_v6&& other) noexcept;

    address_v6& operator=(
        const address_v6& other) noexcept;

    address_v6& operator=(address_v6&& other) noexcept;

    unsigned long scope_id() const noexcept
    {
        return scope_id_;
    }

    void scope_id(unsigned long id) noexcept
    {
        scope_id_ = id;
    }

    bytes_type to_bytes() const noexcept;

    std::string to_string() const;

    static address_v6 from_string(const char* str);

    static address_v6 from_string(const std::string& str);

    address_v4 to_v4() const;

    bool is_loopback() const noexcept;

    bool is_unspecified() const noexcept;

    bool is_link_local() const noexcept;

    bool is_site_local() const noexcept;

    bool is_v4_mapped() const noexcept;

    bool is_v4_compatible() const;

    bool is_multicast() const noexcept;

    bool is_multicast_global() const noexcept;

    bool is_multicast_link_local() const noexcept;

    bool is_multicast_node_local() const noexcept;

    bool is_multicast_org_local() const noexcept;

    bool is_multicast_site_local() const noexcept;

    friend bool operator==(const address_v6& a1,
        const address_v6& a2) noexcept;

    friend bool operator!=(const address_v6& a1,
        const address_v6& a2) noexcept
    {
        return !(a1 == a2);
    }

    friend bool operator<(const address_v6& a1,
        const address_v6& a2) noexcept;

    friend bool operator>(const address_v6& a1,
        const address_v6& a2) noexcept
    {
        return a2 < a1;
    }

    friend bool operator<=(const address_v6& a1,
        const address_v6& a2) noexcept
    {
        return !(a2 < a1);
    }

    friend bool operator>=(const address_v6& a1,
        const address_v6& a2) noexcept
    {
        return !(a1 < a2);
    }

    static address_v6 any() noexcept
    {
        return address_v6();
    }

    static address_v6 loopback() noexcept;

    static address_v6 v4_mapped(const address_v4& addr);

    static address_v6 v4_compatible(const address_v4& addr);

private:
    in6_addr_type addr_;

    unsigned long scope_id_;
};

inline address_v6 make_address_v6(const address_v6::bytes_type& bytes,
    unsigned long scope_id = 0)
{
    return address_v6(bytes, scope_id);
}

address_v6 make_address_v6(const char* str) noexcept;

address_v6 make_address_v6(const std::string& str) noexcept;

enum v4_mapped_t { v4_mapped };

address_v4 make_address_v4(
    v4_mapped_t, const address_v6& v6_addr);

address_v6 make_address_v6(
    v4_mapped_t, const address_v4& v4_addr);

} // namespace net