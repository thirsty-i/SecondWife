#pragma once

#include "net/socket_net_basic.h"
#include "net/socket_acceptor.h"
#include "mtl/design_pattern/singleton.h"
#include "gate_message_handler.h"


class gate_net final
    : public socket_net_basic<role, gate_message_handler>
    , public mtl::singleton<gate_net>
{
public:
    gate_net();
    //void send(user_session_ptr r, std::shared_ptr<msg_header> msg);
    bool start();
private:
private:
    socket_acceptor_ptr socket_acceptor_;
private:
    friend mtl::singleton<gate_net>;
};