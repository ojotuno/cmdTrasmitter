#include <chrono>
#include <thread>
#include <iostream>
#include <zmq.hpp>
#include <memory>
#include "command.hpp"

int main() 
{
    using namespace std::chrono_literals;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    Command cmd;

    for (;;) 
    {
        zmq::message_t request;

        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        
        std::memcpy(&cmd, request.data(), sizeof(cmd));
        std::cout << "Received command:  " << std::endl;
        std::cout << "acceleration -> " << cmd.data.acceleration << "\n";
        std::cout << "speed -> " << cmd.data.speed << "\n";
        std::cout << "wheel -> " << cmd.data.wheel << "\n";
        auto latency = now() - cmd.header.timestamp;
        std::cout << "------Latency " << std::to_string(latency ) << " ns -----\n";
        std::cout << "------Latency " << std::to_string(latency /1'000'000) << " ms -----\n";

        // simulate work
        std::this_thread::sleep_for(1s);

        // send the reply to the client
        ACK ack {.type = ack_t::ok, .timestamp = now()};
        socket.send(zmq::buffer(&ack, sizeof(ack)), zmq::send_flags::none);
    }

    return 0;
}