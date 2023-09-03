#pragma once

#include <optional>
#include <span>

namespace ip
{

class Packet final
{
    const std::span<const uint8_t> data;
    Packet(std::span<const uint8_t> data) noexcept;

  public:
    static std::optional<Packet> find_first(std::span<const uint8_t> data) noexcept;
    static std::optional<Packet> parse_at(std::span<const uint8_t> data) noexcept;

    uint8_t get_version() const noexcept;
    const std::span<const uint8_t> get_data() const noexcept;
};

size_t find_series(std::span<uint8_t> data, std::span<Packet> output) noexcept;

} // namespace ip
