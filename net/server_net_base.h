#pragma once

#include "session_map.h"

template <class UserSession, class MessageHandler>
class server_net_base
{
public:
    using session_map_t = session_map<size_t, UserSession>;
public:
    const MessageHandler& message_handler()
    { 
        return message_handler_; 
    }

    const session_map_t& session_map()
    {
        return session_map_;
    }
protected:
    session_map_t session_map_;
    MessageHandler message_handler_;
};