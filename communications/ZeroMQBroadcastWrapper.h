#ifndef ZEROMQ_BROADCAST_WRAPPER_H
#define ZEROMQ_BROADCAST_WRAPPER_H

#include <zmq.hpp>
#include <string>

class ZeroMQBroadcastWrapper {
public:
    // Constructor for the publisher (broadcaster)
    ZeroMQBroadcastWrapper(const std::string& address, bool is_publisher)
        : context(1), socket(context, is_publisher ? ZMQ_PUB : ZMQ_SUB) {
        if (is_publisher) {
            socket.bind(address);  // Bind to an address to broadcast
        } else {
            socket.connect(address);  // Connect to the address to receive broadcasts
            socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);  // Subscribe to all messages
        }
    }

    // Method to broadcast a message (for the publisher)
    void broadcastMessage(const std::string& message) {
        zmq::message_t zmq_message(message.size());
        memcpy(zmq_message.data(), message.c_str(), message.size());
        socket.send(zmq_message, zmq::send_flags::none);
    }

    // Method to receive a broadcast message (for the subscriber)
    std::string receiveBroadcast() {
        zmq::message_t zmq_message;
        socket.recv(zmq_message, zmq::recv_flags::none);
        return std::string(static_cast<char*>(zmq_message.data()), zmq_message.size());
    }

    // Method to close the connection
    void close() {
        socket.close();
    }

private:
    zmq::context_t context;
    zmq::socket_t socket;
};

#endif // ZEROMQ_BROADCAST_WRAPPER_H
