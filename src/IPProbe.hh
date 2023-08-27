#pragma once

#include <span>

#include <xnet/ip/IP.hh>

namespace ip
{
bool validate_ipv4_at(std::span<const uint8_t> data) noexcept;
bool validate_ipv6_at(std::span<const uint8_t> data) noexcept;
} // namespace ip
