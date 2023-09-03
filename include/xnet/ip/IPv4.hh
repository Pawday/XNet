#pragma once

#include "IP.hh"

#include "v4/TOS.hh"
#include "v4/Option.hh"

namespace ip::v4
{

class TypeOfService final
{
    const uint8_t data;

  public:
    explicit TypeOfService(const uint8_t data) noexcept;

    tos::Precedence get_precedence() const noexcept;
    tos::Delay get_delay() const noexcept;
    tos::Throughput get_throughput() const noexcept;
    tos::Relibility get_relibility() const noexcept;
};

class Flags final
{
    const uint8_t data;

    Flags(const uint8_t data) noexcept;

  public:
    // Validate and encode ipv4 flags from value in format
    // 0b111_00000
    //   ^^^ ^^^^^
    //    |    |
    //    |    +--- anything (ignored)
    //    |
    //    +--------  Flags value
    //
    static std::optional<Flags> try_encode(const uint8_t value) noexcept;

    bool may_fragment() const noexcept;
    bool is_last_fragment() const noexcept;
};





class Option final
{
    const std::span<const uint8_t> data;

  public:
    Option(std::span<const uint8_t> data) noexcept;

    bool is_final() const noexcept;
    option::Type get_type() const noexcept;
    uint8_t get_size() const noexcept;
    uint8_t get_payload_size() const noexcept;
};

class Addres final
{
    const uint32_t addres;
    explicit Addres(const uint32_t addres) noexcept;

  public:
    static Addres from_msbf(const std::span<const uint8_t, 4> data) noexcept;
    uint32_t host_order() const noexcept;
};

class Packet final
{
    const std::span<const uint8_t> data;
    Packet(std::span<const uint8_t> data) noexcept;

  public:
    static bool is_ipv4(const ip::Packet ip_packet) noexcept;
    static std::optional<Packet> cast_from(const ip::Packet ip_packet) noexcept;

    // In 32 bit words (typycal value is 5)
    uint8_t get_IHL() const noexcept;

    TypeOfService get_type_of_service() const noexcept;
    uint16_t get_total_length() const noexcept;
    uint16_t get_identification() const noexcept;
    std::optional<Flags> get_flags() const noexcept;
    uint16_t get_fragment_offset() const noexcept;
    uint8_t get_TTL() const noexcept;
    uint8_t get_protocol_num() const noexcept;
    uint16_t get_header_checksum() const noexcept;
    Addres get_src() const noexcept;
    Addres get_dst() const noexcept;

    uint8_t get_options_size() const noexcept;
    uint8_t get_options_amount() const noexcept;
    std::optional<ip::v4::Option> get_option(uint8_t option_index) const noexcept;

};

/*
 * RFC 791:
 * "The minimum value for a correct header is 20
 */
constexpr uint8_t minimal_header_length = 5 * sizeof(uint32_t);

} // namespace ip::v4
