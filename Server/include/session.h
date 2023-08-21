#pragma once

#include <deque>

#include <boost/asio.hpp>

#include "message.h"
#include "participant.h"
#include "room.h"

using boost::asio::ip::tcp;


class Session :
        public Participant,
        public std::enable_shared_from_this<Session>
{
private:
    tcp::socket socket_;
    Room & room_;
    Message read_msg_;
    std::deque<Message> write_msgs_;

public:
    Session(tcp::socket socket, Room & room);

    void start();
    void deliver(const Message & msg);

    // Utilities
    std::string getLocalTime() const;
    tcp::endpoint clientEndpoint() const;
    std::string parseAddressAndPort(tcp::endpoint ep) const;

private:

    void read();
    void handle_read(const boost::system::error_code & error);

    void write();
    void handle_write(const boost::system::error_code & error);

    void parseCommand();
};
