#pragma once

#include "session_map.h"

template <class UserSession, class MessageHandler>
class server_net_base
{
public:
    using session_map_t = session_map<size_t, UserSession>;
public:
    const MessageHandler::pointer message_handler() 
    { 
        return std::addressof(message_handler_); 
    }

    const session_map_t::pointer session_map()
    {
        return std::addressof(session_map_);
    }

private:
    session_map_t::pointer session_map_;
    MessageHandler::pointer message_handler_;
};