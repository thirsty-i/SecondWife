#pragma once

#include "net/server_net_base.h"
#include "net/socket_acceptor.h"
#include "mtl/design_pattern/singleton.h"
#include "gate_message_handler.h"

class role
    : public user_session_base
{
public:
};

class gate_net final
    : public server_net_base<role, gate_message_handler>
    , public mtl::singleton<gate_net>
{
public:
    gate_net();
private:
    _on_new_connection(socket_session_ptr& new_session);
    _on_close(socket_acceptor_ptr session);
private:
    socket_acceptor_ptr socket_acceptor_;
private:
    friend mtl::singleton<gate_net>;
};