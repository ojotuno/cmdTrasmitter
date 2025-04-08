#pragma once

#include <stdint.h>
#include <chrono>


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

enum class ack_t { ok, error }; //add more

std::string ack_t2Str(ack_t t)
{
    switch (t)
    {
    case ack_t::ok: return "ok";    
    case ack_t::error: return "error";
    default:
        return "none";
    }
}

struct ACK
{
    ack_t type;
    uint64_t  timestamp; // ns resolution
};

std::string ack2Str(ACK const& ack)
{
    return "type ->  " + ack_t2Str(ack.type) + "\nts -> " + std::to_string(ack.timestamp);
}

// shoudn't be here
static inline auto now() noexcept -> uint64_t {
    namespace ts = std::chrono;
    return ts::duration_cast<ts::nanoseconds>(ts::steady_clock::now().time_since_epoch()).count();
}

static inline auto toMs(std::chrono::nanoseconds ns)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(ns).count();
}

