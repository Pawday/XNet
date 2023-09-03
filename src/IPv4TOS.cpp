#include <xnet/ip/IPv4.hh>

namespace ip::v4
{
using namespace tos;

tos::Precedence TypeOfService::get_precedence() const noexcept
{
    uint8_t precedence = ((this->data & 0b11100000) >> 5) & 0b111;
    switch (precedence)
    {
    case 0:
        return Precedence::ROUTINE;
    case 1:
        return Precedence::PRIORITY;
    case 2:
        return Precedence::IMMEDIATE;
    case 3:
        return Precedence::FLASH;
    case 4:
        return Precedence::FLASH_OVERRIDE;
    case 5:
        return Precedence::CRITIC_ECP;
    case 6:
        return Precedence::INTERNETWORK_CONTROL;
    case 7:
        return Precedence::NETWORK_CONTROL;
    }

    std::unreachable();
}

Delay TypeOfService::get_delay() const noexcept
{
    if ((this->data & 0b00010000) == 0)
    {
        return Delay::NORMAL;
    }

    return Delay::LOW;
}

Throughput TypeOfService::get_throughput() const noexcept
{
    using namespace tos;
    if ((this->data & 0b00001000) == 0)
    {
        return Throughput::NORMAL;
    }

    return Throughput::HIGH;
}

Relibility TypeOfService::get_relibility() const noexcept
{
    if ((this->data & 0b00000100) == 0)
    {
        return Relibility::NORMAL;
    }

    return Relibility::HIGH;
}

} // namespace ip::v4
