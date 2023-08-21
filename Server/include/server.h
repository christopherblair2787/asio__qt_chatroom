#pragma once

#include <boost/asio.hpp>

#include "room.h"
#include "session.h"

using boost::asio::ip::tcp;


class Server
{
private:
    boost::asio::io_context & io_context_;
    tcp::acceptor acceptor_;
    Room room_;

public:
    Server(boost::asio::io_context & io_context, const tcp::endpoint & endpoint);

private:
    void accept();
};
