#pragma once
#include "../message_handlers/gate_message_handler.h"
#include "net/socket_acceptor.h"
#include "mtl/design_pattern/singleton.h"

class role
{

};

class gate_service final
    : public mtl::singleton<gate_service>
{
public:
    gate_service();

    role* find_role(int session_id) 
    { 
        auto iter = table_.find(session_id);
        if(iter == table_.end())
            return 0;

        return iter->second;
    }
    
    const gate_message_handler_ptr message_handler() { return message_handler_; }
private:
    gate_message_handler_ptr message_handler_;
    socket_acceptor_ptr socket_acceptor_;

    std::map<int, role*> table_;
private:
    friend mtl::singleton<gate_service>;
};