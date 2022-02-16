#pragma once

#include <memory>
#include "message_handler.h"

class socket_session
{
public:
    using pointer = std::shared_ptr<socket_session>;

    void set_message_handler(message_handler* handler);
private:
    message_handler* handlers_;
};