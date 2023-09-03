#include <xnet/ip/IP.hh>

#include <utility>

#include "IPProbe.hh"

namespace ip
{

Packet::Packet(std::span<const uint8_t> data) noexcept : data(data)
{
}

uint8_t Packet::get_version() const noexcept
{
    return (this->data[0] & 0xf0) >> 4;
}

const std::span<const uint8_t> Packet::get_data() const noexcept
{
    return this->data;
}

static std::optional<size_t> get_uint4_offset(std::span<const uint8_t> data, uint8_t uint4_value)
{
    if (uint4_value > 15)
    {
        std::unreachable();
    }

    size_t pos = 0;
    for (const auto num : data)
    {
        if (((num & 0xf0) >> 4) != uint4_value)
        {
            pos++;
            continue;
        }
        return pos;
    }

    return {};
}

std::optional<Packet> Packet::parse_at(std::span<const uint8_t> data) noexcept
{
    bool is_ipv4_valid = validate_ipv4_at(data);
    bool is_ipv6_valid = validate_ipv6_at(data);

    if (!(is_ipv4_valid || is_ipv6_valid))
    {
        return {};
    }

    return Packet(data);
}

std::optional<Packet> Packet::find_first(std::span<const uint8_t> data) noexcept
{
    const auto ipv4_header_pos = get_uint4_offset(data, 4);
    const auto ipv6_header_pos = get_uint4_offset(data, 6);

    if (!(ipv4_header_pos || ipv6_header_pos))
    {
        return {};
    }

    auto is_ipv4_valid = false;
    auto is_ipv6_valid = false;

    if (ipv4_header_pos && ipv6_header_pos)
    {
        is_ipv4_valid = validate_ipv4_at(data.subspan(*ipv4_header_pos));
        is_ipv6_valid = validate_ipv6_at(data.subspan(*ipv4_header_pos));

        if (*ipv4_header_pos < *ipv6_header_pos)
        {
            if (is_ipv4_valid)
            {
                return Packet(data.subspan(*ipv4_header_pos));
            }

            if (is_ipv6_valid)
            {
                // TODO: invoke ipv6 wrapper
                return {};
            }

            return {};
        }

        if (is_ipv6_valid)
        {
            // TODO: invoke ipv6 wrapper
            return {};
        }

        if (is_ipv4_valid)
        {
            return Packet(data.subspan(*ipv4_header_pos));
            return {};
        }

        return {};
    }

    if (ipv4_header_pos.has_value() &&
        validate_ipv4_at(data.subspan(*ipv4_header_pos)))
    {
        return Packet(data.subspan(*ipv4_header_pos));
        return {};
    }

    if (ipv6_header_pos.has_value())
    {
        // TODO: invoke ipv6 wrapper
        return {};
    }

    return {};
}
} // namespace ip
