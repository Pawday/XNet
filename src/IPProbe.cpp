#include "IPProbe.hh"


#include <cstdint>
#include <utility>

#include <xnet/ip/IPv4.hh>
#include <xnet/ip/IPv6.hh>


namespace ip
{

static bool verify_ipv4_checksum(std::span<const uint8_t> data)
{
    if (data.size() == 0)
    {
	std::unreachable();
        return false;
    }

    if (data.size() < ip::v4::minimal_header_length)
    {
        return false;
    }

    // if data size is not even (16 bits * N) we cannot
    //    verify RFC791 Header Checksum 
    if ((data.size() & 1) != 0)
    {
        return false;
    }

    size_t data_length_in_u16 = data.size() / sizeof(uint16_t);

    constexpr uint8_t ipv4_checksum_position = 5;

    uint16_t checksum = 0;
    for (size_t word_index = 0; word_index < ipv4_checksum_position; word_index++)
    {
        uint16_t next_word = data[word_index * sizeof(uint16_t) + 0] << 8 |
            data[word_index * sizeof(uint16_t) + 1];
        if (next_word > UINT16_MAX - checksum)
        {
            checksum++;
        }
        checksum += next_word;
    }

    uint16_t checksum_from_header = 0;
    checksum_from_header |= data[ipv4_checksum_position * sizeof(uint16_t) + 0] << 8;
    checksum_from_header |= data[ipv4_checksum_position * sizeof(uint16_t) + 1] << 0;

    for (size_t word_index = ipv4_checksum_position + 1; word_index < data_length_in_u16; word_index++)
    {
        uint16_t next_word =
            data[word_index * sizeof(uint16_t) + 0] << 8 | data[word_index * sizeof(uint16_t) + 1];
        if (next_word > UINT16_MAX - checksum)
        {
            checksum++;
        }
        checksum += next_word;
    }

    uint16_t checksum_compliment = ~checksum;
    return checksum_compliment == checksum_from_header;
}

bool validate_ipv4_at(std::span<const uint8_t> data) noexcept
{
    if (data.size() < ip::v4::minimal_header_length)
    {
        return {};
    }

    // Version + Internet Header Length
    uint8_t ver_IHL_surrogate = data[0];
    if ((ver_IHL_surrogate & 0xf0) != 0x40)
    {                      //           ^
                           //           |
                           //           +----- value for ipv4
        return false;
    }

    uint8_t IHL = ver_IHL_surrogate & 0x0f;

    /*
     * RFC 791:
     * "The minimum value for a correct IHL is 5"
    */
    if (IHL < 5)
    {
        return false;
    }

    if (data.size() < IHL * 4)
    {
        return false;
    }

    if (!verify_ipv4_checksum(data.subspan(0, IHL * sizeof(uint32_t))))
    {
        return false;
    }

    uint16_t total_length = 0;
    total_length |= data[2] << 8;
    total_length |= data[3];

    if (data.size() < total_length)
    {
        return false;
    }

    
    constexpr uint8_t ipv4_flags_position = 6;
    // data[ipv4_flags_position]:
    // 0b000_00000
    //    ^    ^
    //    |    |
    //    |    +--- first part of fragment Offset
    //    |
    //    +----- Flags

    /*
     * RFC 791:
     * "Bit 0: reserved, must be zero"
    */
    if ((data[ipv4_flags_position] & 0b10000000) != 0)
    {
        return false;
    }

    return true;
}

bool validate_ipv6_at(std::span<const uint8_t> data) noexcept
{
    (void)data;
    return false;
}

} // namespace ip
