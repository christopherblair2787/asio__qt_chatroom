#pragma once

#include <deque>

#include <boost/asio.hpp>

#include "message.h"

using boost::asio::ip::tcp;

class Client
{
private:
    boost::asio::io_context & io_context_;
    tcp::socket socket_;

    Message read_msg_;
    std::deque<Message> write_msgs_;

    std::string username_;

public:
    Client(boost::asio::io_service & io_context,
           const tcp::resolver::results_type & endpoints,
           const std::string & username);

    void write(const Message & msg);

    void close();

    void command(const Message & msg);

private:

    void connect(const tcp::resolver::results_type & endpoints);
    void handle_connection(const boost::system::error_code & error);

    void read();
    void handle_read(const boost::system::error_code & error);

    void write();
    void handle_write(const boost::system::error_code & error);

    void handle_close();
};
