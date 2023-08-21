#pragma once

#include <iostream>
#include <cstring>

class Message
{
private:
    enum Pad { MAX_LENGTH = 2048 };
    char data_[MAX_LENGTH];

public:

    // Constructors
    Message() = default;

    Message(const char * msg)
    {
        copy(msg);
    }

    Message(const std::string & msg)
    {
        copy(msg.c_str());
    }

    Message(const Message & msg)
    {
        copy(msg.data_);
    }


    Message & operator=(const char * msg)
    {
        copy(msg);

        return * this;
    }

    Message & operator=(const std::string & msg)
    {
        copy(msg.c_str());

        return * this;
    }

    Message & operator=(const Message & msg)
    {
        copy(msg.data_);

        return * this;
    }


    // Getters and Setters

    void copy(const char * msg)
    {
        std::strncpy(data_, msg, Pad::MAX_LENGTH);
    }

    void copy(const std::string & msg)
    {
        copy(msg.c_str());
    }


    char * data()
    {
        return data_;
    }

    const char * data() const
    {
        return data_;
    }


    // Utilities

    size_t max_length() const
    {
        return Pad::MAX_LENGTH;
    }


    bool empty() const
    {
        return (data_[0] == '\n') || (data_[0] == '\0');
    }


    // Operators

    bool operator==(const char * str) const
    {
        return (std::strcmp(data_, str) == 0);
    }


    bool operator==(const std::string & str) const
    {
        return (*this == str.c_str());
    }


    bool operator==(const Message & msg) const
    {
        return (*this == msg.data_);
    }


    friend std::ostream & operator<<(std::ostream & os, const Message & msg)
    {
        os << msg.data() << std::endl;

        return os;
    }
};
