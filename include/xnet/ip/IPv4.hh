#pragma once

#include "IP.hh"

#include <optional>


namespace ip::v4
{

class Packet final
{
    std::span<const uint8_t> data;
};

/*
 * RFC 791:
 * "The minimum value for a correct header is 20
 */
constexpr uint8_t minimal_header_length = 5 * sizeof(uint32_t);


class Addres
{
    std::span<uint8_t, 4> data;
};

namespace tos
{

enum class Precedence
{
    ROUTINE,
    PRIORITY,
    IMMEDIATE,
    FLASH,
    FLASH_OVERRIDE,
    CRITIC_ECP,
    INTERNETWORK_CONTROL,
    NETWORK_CONTROL
};

enum class Delay
{
    NORMAL,
    LOW
};

enum class Throughput
{
    NORMAL,
    HIGH
};

enum class Relibility
{
    NORMAL,
    HIGH
};

class Type final
{
    Precedence precedence; //< +
    Delay delay;           //< |
    Throughput throughput; //< |
    Relibility relibility; //< |
//                             |
//            +----------------+
//            |    (encodes)
//            |
    uint8_t masked_byte;

    bool validate(void);


public:
    explicit Type(uint8_t masked_byte);
    void encode(const uint8_t new_mask);
    uint8_t decode(void);

};

} // namespace tos

std::optional<Packet> try_cast_from(ip::Packet);


} // namespace ip::v4
