#pragma once

class message_handler
{
    virtual void on_accepted() = 0;
    virtual void on_connected() = 0;
    virtual void on_recv() = 0;
    virtual void on_disconnected() = 0;
};