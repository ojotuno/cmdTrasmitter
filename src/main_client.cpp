#include <string>
#include <iostream>

#include <zmq.hpp>

#include "command.hpp"

static inline constexpr void fillHeader(Command& cmd, command_t type)
{
    cmd.header.size = sizeof(cmd.data);
    cmd.header.type = type;
    cmd.header.timestamp = now();
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
        
        auto ack = static_cast<ACK*>(reply.data());

        std::cout << "Received ACK\n"; 
        std::cout << ack2Str(*ack);
        std::cout << std::endl;
    }

    return 0;
}