#pragma once

#include <deque>
#include <set>

#include "message.h"
#include "participant.h"


class Room
{
private:
    std::set<participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    std::deque<Message> recent_msgs_;

public:
    void join(participant_ptr participant);

    void leave(participant_ptr participant);

    void deliver(const Message & msg, tcp::endpoint remote_endpoint);

    void sendParticipantsList(participant_ptr remote);
};
