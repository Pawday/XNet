#pragma once

#include <array>
#include <optional>

#include "IP.hh"

namespace ip::v6
{

class Packet final
{
    std::span<const uint8_t> data;
};

class Addres final
{
    std::span<uint8_t, 16> data;
};

std::optional<Packet> try_get_from(ip::Packet);

} // namespace ip::v6
