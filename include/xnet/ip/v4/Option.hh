#pragma once

namespace ip::v4::option
{

// Smallest option take 1 byte
//  1. IHL is 4 bit
//  2. IHL_max value is 15
//  3. IHL_min (without options) is 5
//  Option Max length =
//    = IHL_max - IHL_min
//    = 15 - 5 * sizeof(uint32_t) =
//    = 10 * sizeof(uint32_t) =
//    = 10 * 4
constexpr uint8_t max_count = 40;

enum class Class
{
    CONTROL,
    RESERVED,
    DEBUGGING_AND_MEASUREMENT
};

class Type final
{
    const uint8_t data;

  public:
    Type(const uint8_t data) noexcept;
    bool is_copied() const noexcept;
    Class get_class() const noexcept;
    uint8_t get_number() const noexcept;
};

} // namespace option
