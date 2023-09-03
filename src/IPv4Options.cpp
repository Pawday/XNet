#include <xnet/ip/IPv4.hh>

namespace ip::v4
{

using namespace option;

Option::Option(std::span<const uint8_t> data) noexcept : data(data)
{
}

Type::Type(const uint8_t data) noexcept : data(data)
{
}

bool Option::is_final() const noexcept
{
    const auto type = this->get_type();
    return type.get_class() == option::Class::CONTROL && type.get_number() == 0;
}

option::Type Option::get_type() const noexcept
{
    return Type(this->data[0]);
}

uint8_t Option::get_size() const noexcept
{
    const auto number = this->get_type().get_number();
    if (number == 0 || number == 1)
    {
        return 1;
    }

    return this->data[1];
}


bool Type::is_copied() const noexcept
{
    return !((this->data & 0b10000000) == 0);
}

option::Class Type::get_class() const noexcept
{
    uint8_t class_bits = (this->data & 0b01100000) >> 5;

    switch (class_bits)
    {
    case 0:
        return Class::CONTROL;
    case 1:
        return Class::RESERVED;
    case 2:
        return Class::DEBUGGING_AND_MEASUREMENT;
    case 3:
        return Class::RESERVED;
    }

    std::unreachable();
}

uint8_t Type::get_number() const noexcept
{
    return this->data & 0b00011111;
}

} // namespace ip::v4
