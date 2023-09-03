#include <xnet/ip/IPv4.hh>

#include <utility>
#include <array>

namespace ip::v4
{

Packet::Packet(std::span<const uint8_t> data) noexcept : data(data)
{
}


TypeOfService::TypeOfService(const uint8_t data) noexcept : data(data)
{
}

bool Packet::is_ipv4(const ip::Packet ip_packet) noexcept
{
    return ip_packet.get_version() == 4;
}

std::optional<Packet> Packet::cast_from(const ip::Packet ip_packet) noexcept
{
    if (!is_ipv4(ip_packet))
    {
        return {};
    }

    return Packet(ip_packet.get_data());
}

uint8_t Packet::get_IHL() const noexcept
{
    return this->data[0] & 0x0f;
}

TypeOfService Packet::get_type_of_service() const noexcept
{
    return TypeOfService(this->data[1]);
}

uint16_t Packet::get_total_length() const noexcept
{
    return (this->data[2] << 8) | (this->data[3] << 0);
}

uint16_t Packet::get_identification() const noexcept
{
    return (this->data[4] << 8) | (this->data[5] << 0);
}

std::optional<Flags> Packet::get_flags() const noexcept
{
    return Flags::Flags::try_encode(this->data[6]);
}

uint16_t Packet::get_fragment_offset() const noexcept
{
    return (this->data[6] & 0b00011111) << 8 | this->data[7];
}

uint8_t Packet::get_TTL() const noexcept
{
    return this->data[8];
}

uint8_t Packet::get_protocol_num() const noexcept
{
    return this->data[9];
}

uint16_t Packet::get_header_checksum() const noexcept
{
    return this->data[10] << 8 | this->data[11];
}

Addres Packet::get_src() const noexcept
{
    std::array<uint8_t, 4> addr_data;
    std::copy(
        this->data.subspan(12).begin(), this->data.subspan(12).begin() + 4,
        addr_data.begin()
    );
    return Addres::from_msbf(addr_data);
}

Addres Packet::get_dst() const noexcept
{
    std::array<uint8_t, 4> addr_data;
    std::copy(
        this->data.subspan(16).begin(), this->data.subspan(16).begin() + 4,
        addr_data.begin()
    );
    return Addres::from_msbf(addr_data);
}

uint8_t Packet::get_options_size() const noexcept
{
    uint8_t IHL = this->get_IHL();
    if (IHL <= 5)
    {
        return 0;
    }

    return (IHL - 5) * sizeof(uint32_t);
}

uint8_t Packet::get_options_amount() const noexcept
{
    const std::span<const uint8_t> ipv4_options_data = this->data.subspan(20);
    if (ipv4_options_data.size() != this->get_options_size())
    {
        return 0;
    }

    uint8_t options_so_far = 0;
    uint8_t next_option_offset = 0;
    bool options_out = false;


    while (!options_out && options_so_far <= option::max_count)
    {
        Option next_option(ipv4_options_data.subspan(next_option_offset));

        if (next_option.is_final())
        {
            return options_so_far + 1;
        }

        uint8_t next_option_size = next_option.get_size();

        if (next_option_size == 0 || options_so_far == option::max_count)
        {
            return options_so_far;
        }

        next_option_offset += next_option.get_size();
        options_so_far++;

    }

    return options_so_far;
}

Addres::Addres(const uint32_t addres) noexcept : addres(addres)
{
}

Addres Addres::from_msbf(const std::span<const uint8_t, 4> data) noexcept
{
    uint32_t addres = 0;
    addres |= data[0] << 24;
    addres |= data[1] << 16;
    addres |= data[2] << 8;
    addres |= data[3] << 0;

    return Addres(addres);
}

uint32_t Addres::host_order() const noexcept
{
    return this->addres;
}

Flags::Flags(const uint8_t data) noexcept : data(data)
{
    if ((data & 0b10000000) != 0)
    {
        std::unreachable();
    }
}

std::optional<Flags> Flags::try_encode(const uint8_t value) noexcept
{
    /*
     * RFC 791:
     * "Bit 0: reserved, must be zero"
     */
    if ((value & 0b10000000) != 0)
    {
        return {};
    }

    return Flags(value);
}

bool Flags::may_fragment() const noexcept
{
    return (this->data & 0b01000000) == 0;
}

bool Flags::is_last_fragment() const noexcept
{
    return (this->data & 0b00100000) == 0;
}

} // namespace ip::v4

