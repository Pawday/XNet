#include <xnet/ip/IPv4.hh>

#include <cassert>

namespace ip::v4
{

std::optional<Packet> try_cast_from(ip::Packet)
{
    return {};
}

} // namespace ip::v4

namespace ip::v4::tos
{

Type::Type(uint8_t masked_byte)
{
    this->encode(masked_byte);
}

void Type::encode(const uint8_t masked_byte)
{
    this->masked_byte = masked_byte;

    uint8_t precedence = masked_byte & 0x07;

#if TRUST_COMPILER
    this->precedence = static_cast<Precedence>(precedence);
#else
    switch (precedence)
    {
    case 0:
        this->precedence = Precedence::ROUTINE;
        break;
    case 1:
        this->precedence = Precedence::PRIORITY;
        break;
    case 2:
        this->precedence = Precedence::IMMEDIATE;
        break;
    case 3:
        this->precedence = Precedence::FLASH;
        break;
    case 4:
        this->precedence = Precedence::FLASH_OVERRIDE;
        break;
    case 5:
        this->precedence = Precedence::CRITIC_ECP;
        break;
    case 6:
        this->precedence = Precedence::INTERNETWORK_CONTROL;
        break;
    case 7:
        this->precedence = Precedence::NETWORK_CONTROL;
        break;
    }
#endif

    this->delay = ((masked_byte & 0x08) == 0) ? Delay::NORMAL : Delay::LOW;
    this->throughput =
        ((masked_byte & 0x10) == 0) ? Throughput::NORMAL : Throughput::HIGH;
    this->relibility =
        ((masked_byte & 0x20) == 0) ? Relibility::NORMAL : Relibility::HIGH;
}

} // namespace ip::v4::tos
