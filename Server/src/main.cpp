#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "message.h"
#include "server.h"

using boost::asio::ip::tcp;


int main(int argc, char *argv[])
{
    try
    {
        if(argc < 2)
        {
            std::cout << "Port: <port1> <port2> ...";
        }

        boost::asio::io_context io_context;

        std::list<Server> servers;
        for(int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}
