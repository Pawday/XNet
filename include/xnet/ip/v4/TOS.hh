#pragma once

namespace ip::v4::tos
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

} // namespace ip::v4::tos
