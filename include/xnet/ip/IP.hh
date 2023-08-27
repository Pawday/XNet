#pragma once

#include <optional>
#include <span>

namespace ip
{

class Packet final
{
    std::span<const uint8_t> data;
};

std::optional<Packet> find_first(std::span<const uint8_t> data) noexcept;
size_t find_series(std::span<uint8_t> data, std::span<Packet> output) noexcept;

} // namespace ip
