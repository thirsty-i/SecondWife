#pragma once

#include <memory>
#include "message_handler.h"

class user_session_base;

class socket_session
{
public:
    void set_message_handler(const message_handler_ptr handler);

private:
    message_handler_ptr handlers_;
};

typedef std::shared_ptr<socket_session> socket_session_ptr;