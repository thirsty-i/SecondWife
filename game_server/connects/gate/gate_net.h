#pragma once

#include "net/server_net_base.h"
#include "net/socket_acceptor.h"
#include "mtl/design_pattern/singleton.h"

class role
    : public user_session_base
{
public:
    using pointer = std::shared_ptr<role>;
};

class gate_net final
    : public server_net_base<role, gate_message_handler>
    , public mtl::singleton<gate_net>
{
public:
    gate_net();
private:
    socket_acceptor_ptr socket_acceptor_;
private:
    friend mtl::singleton<gate_net>;
};