#pragma once

#include <stdint.h>

enum class command_t : uint32_t {
    none = 0,
    emergency_break,
    throtle,
    turn_wheel
};

// the order of the elements is to avoid padding
struct CommandHeader
{
    uint64_t  timestamp; // ns resolution
    uint32_t size = 0;
    command_t type = command_t::none;
};

struct CommandData
{
    float speed = .0f;
    float acceleration = 0.f;
    float wheel = 0.f;
};

struct Command {
    CommandHeader header = {};
    CommandData data;
};