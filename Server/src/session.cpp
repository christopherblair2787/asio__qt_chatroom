#include <iostream>
#include <cstring>
#include <ctime>

#include <boost/bind.hpp>

#include "session.h"


Session::Session(tcp::socket socket, Room & room) :
    socket_(std::move(socket)),
    room_(room)
{
}


void Session::start()
{
    room_.join(shared_from_this());

    // Mandar un mensaje al resto de participantes para indicar que se ha unido a la sala
    Message connection_message(parseAddressAndPort(clientEndpoint()) +
                               " se ha conectado al servidor.");

    std::cout << connection_message;
    room_.deliver(connection_message, clientEndpoint());

    read();
}


void Session::deliver(const Message & msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if(!write_in_progress)
    {
        write();
    }
}


std::string Session::getLocalTime() const
{
    char cstr[30];
    std::time_t t = std::time(0);
    std::strftime(cstr, sizeof(cstr), "[%a %x %X] > ", std::localtime(&t));
    return std::string(cstr);
}

tcp::endpoint Session::clientEndpoint() const
{
    return socket_.remote_endpoint();
}


std::string Session::parseAddressAndPort(tcp::endpoint ep) const
{
    return std::string(ep.address().to_string() + ":" + std::to_string(ep.port()));
}


void Session::read()
{
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_.data(),
                                                read_msg_.max_length()),
                            boost::bind(&Session::handle_read, shared_from_this(),
                                        boost::asio::placeholders::error));
}


void Session::handle_read(const boost::system::error_code & error)
{
    if(!error)
    {
        if(std::strncmp(read_msg_.data(), "?:", 2) == 0)
        {
            parseCommand();
        }
        else
        {
            if(!read_msg_.empty())
            {
                Message formatted_msg(getLocalTime() + getUsername() + " dijo: " + read_msg_.data());
                room_.deliver(formatted_msg, socket_.remote_endpoint());
            }
        }

        read();
    }
    else //Disconnect client in case of error
    {
        Message disconnection_message(getUsername() + " (" + parseAddressAndPort(clientEndpoint()) +
                                      ") se ha desconectado del servidor.");

        std::cout << disconnection_message;
        room_.deliver(disconnection_message, clientEndpoint());

        room_.leave(shared_from_this());
    }
}


void Session::write()
{
    boost::asio::async_write(socket_,
                             boost::asio::buffer(write_msgs_.front().data(),
                                                 write_msgs_.front().max_length()),
                             boost::bind(&Session::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error));
}


void Session::handle_write(const boost::system::error_code & error)
{
    if(!error)
    {
        write_msgs_.pop_front();

        if(!write_msgs_.empty())
        {
            write();
        }
    }
    else
        room_.leave(shared_from_this());
}


void Session::parseCommand()
{
    std::cout << "SERVER: Command received: " << read_msg_.data() << std::endl;

    if(std::strcmp(read_msg_.data(), "?:lista") == 0)
    {
        Message info("Lista de participantes de la sala:");
        deliver(info);
        room_.sendParticipantsList(shared_from_this());
    }
    else if (std::strncmp(read_msg_.data(), "?:nombre", 8) == 0)
    {
        setUsername(read_msg_.data() + 8);
        Message tell_username("(" + parseAddressAndPort(clientEndpoint()) + ") Nuevo nombre de usuario: " + getUsername());
        room_.deliver(tell_username, clientEndpoint());
    }
}
