#pragma once

#include <memory>

class message_handler
{
public:
    virtual void on_connected() = 0;
    virtual void on_recv() = 0;
    virtual void on_disconnected() = 0;
};

typedef std::shared_ptr<message_handler> message_handler_ptr;