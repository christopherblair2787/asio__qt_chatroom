#include "room.h"


void Room::join(participant_ptr participant)
{
    participants_.insert(participant);

    for(auto msg : recent_msgs_)
        participant->deliver(msg);
}

void Room::leave(participant_ptr participant)
{
    participants_.erase(participant);
}

void Room::deliver(const Message & msg, tcp::endpoint remote_endpoint)
{
    if(!msg.empty())
    {
        recent_msgs_.push_back(msg);
        while(recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for(auto participant : participants_)
            if(participant->clientEndpoint() != remote_endpoint)
                participant->deliver(msg);
    }
}


void Room::sendParticipantsList(participant_ptr remote)
{
    for(auto & participant : participants_)
    {
        tcp::endpoint ep = participant->clientEndpoint();
        Message part_info(participant->getUsername() + ". (" + ep.address().to_string() + ":" + std::to_string(ep.port()) + ")");
        remote->deliver(part_info);
    }

}
