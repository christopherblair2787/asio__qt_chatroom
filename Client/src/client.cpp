#include <iostream>
#include <string>


#include <boost/bind.hpp>

#include "message.h"
#include "client.h"


Client::Client(boost::asio::io_service & io_context,
               const tcp::resolver::results_type & endpoints,
               const std::string & username) :
    io_context_(io_context),
    socket_(io_context),
    username_(username)
{
    connect(endpoints);
}


void Client::write(const Message & msg)
{
    boost::asio::post(io_context_,
        [this, msg]()
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if(!write_in_progress)
            write();
    });
}


void Client::close()
{
    boost::asio::post(io_context_, boost::bind(&Client::handle_close, this));
}


void Client::command(const Message & msg)
{
    if(msg == "?:lista")
        write(Message("?:lista"));
    else if(std::strncmp(msg.data(), "?:nombre", 8) == 0)
    {
        std::cout << "Nuevo nombre de usuario?" << std::endl;
        std::cin >> username_;
        Message tell_username("?:nombre" + username_);
        write(tell_username);
    }
    else if(msg == "?:help")
    {
        std::cout << "Lista de comandos: " << std::endl;
        std::cout << "?:nombre -> Cambia el nombre de usuario." << std::endl;
        std::cout << "?:lista -> Muestra un listado con los usuarios conectados al servidor." << std::endl;
        std::cout << "?:salir -> Cerrar el programa." << std::endl;
    }
    else
        std::cout << "No se ha encontrado el comando. Escriba ?:help para mas informacion." << std::endl;
}


void Client::connect(const tcp::resolver::results_type & endpoints)
{
    boost::asio::async_connect(socket_,endpoints,
                               boost::bind(&Client::handle_connection, this,
                                           boost::asio::placeholders::error));
}

void Client::handle_connection(const boost::system::error_code & error)
{
    if(!error)
    {
        std::cout << "Te has conectado al servidor "
                  << socket_.remote_endpoint().address() << ":"
                  << socket_.remote_endpoint().port() << std::endl;

        Message tell_username("?:nombre" + username_);
        write(tell_username);

        read();
    }
}


void Client::read()
{
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_.data(), read_msg_.max_length()),
                            boost::bind(&Client::handle_read, this, boost::asio::placeholders::error));
}

void Client::handle_read(const boost::system::error_code & error)
{
    if(!error)
    {
        std::cout << read_msg_;

        read();
    }
    else
    {
        socket_.close();
    }
}


void Client::write()
{
    boost::asio::async_write(socket_,
                             boost::asio::buffer(write_msgs_.front().data(),
                                                 write_msgs_.front().max_length()),
                             boost::bind(&Client::handle_write, this, boost::asio::placeholders::error));
}

void Client::handle_write(const boost::system::error_code & error)
{
    if(!error)
    {
        write_msgs_.pop_front();
        if(!write_msgs_.empty())
            write();
    }
    else
    {
        socket_.close();
    }
}


void Client::handle_close()
{
    socket_.close();
}

