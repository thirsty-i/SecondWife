#pragma once
#include "session_map.h"

template <class UserSession, class MessageHandler>
class socket_net_basic
{
protected:
    using user_session_t = UserSession;
    using session_map_t = session_map<size_t, user_session_t>;
    using user_session_ptr = std::shared_ptr<user_session_t>;
    using message_handler_t = MessageHandler;
public:
    const message_handler_t& message_handler() const
    { 
        return message_handler_; 
    }

    const session_map_t& get_session_map() const
    {
        return session_map_;
    }

protected:
    session_map_t session_map_;
    message_handler_t message_handler_;
};