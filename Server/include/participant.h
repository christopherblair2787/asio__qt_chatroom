#pragma once

#include <memory>

#include <string>
#include <boost/asio.hpp>

#include "message.h"

using boost::asio::ip::tcp;


class Participant
{
private:
    std::string username_;
public:
    virtual ~Participant() = default;

    virtual void deliver(const Message & msg) = 0;

    virtual tcp::endpoint clientEndpoint() const = 0;

    std::string getUsername() const { return username_; }

    void setUsername(const char * username) { username_ = username; }

};

typedef std::shared_ptr<Participant> participant_ptr;
