#include <string>
#include <iostream>
#include <chrono>
#include <zmq.hpp>

#include "command.hpp"

static inline constexpr auto nowTimestamp() noexcept -> uint64_t {
    namespace ts = std::chrono;
    return ts::duration_cast<ts::nanoseconds>(ts::steady_clock::now().time_since_epoch()).count();
}

static inline constexpr void fillHeader(Command& cmd, command_t type)
{
    cmd.header.size = sizeof(cmd.data);
    cmd.header.type = type;
    cmd.header.timestamp = nowTimestamp();
}

int main()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");

    // set up some static data to send
    Command cmd;

    float dummyData = .0f;

    for (auto request_num = 0; request_num < 10; ++request_num) 
    {
        dummyData += 0.3;
        // send the request message
        std::cout << "Sending command " << request_num << "..." << std::endl;
       
        cmd.data.acceleration = dummyData;
        cmd.data.speed = dummyData;
        cmd.data.wheel = dummyData;
        fillHeader(cmd, command_t::throtle);
     
        socket.send(zmq::buffer(&cmd, sizeof(cmd)), zmq::send_flags::none);
        
        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);

        std::cout << "Received " << reply.to_string(); 
        std::cout << " (" << request_num << ")";
        std::cout << std::endl;
    }

    return 0;
}