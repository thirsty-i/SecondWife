#include "net/message_handler.h"
#include "common/noncopyable.h"

class gate_service;

class gate_message_handler final
    : public  message_handler
    , private noncopyable
{
public:
    virtual void on_connected();

    virtual void on_recv();

    virtual void on_disconnected();
public:
    gate_message_handler(gate_service*);

private:
    gate_service* service_;
};

using gate_message_handler_ptr = std::shared_ptr<gate_message_handler>;