#pragma once

#include "net/message_handler_basic.hpp"
#include "common/message/msg_def.h"

class role
	: public user_session_base
{
public:
};

class gate_message_handler final
    : public message_handler_basic<role, msg_header>
{
public:
    gate_message_handler();

    void do_test_net(user_session_t session, int32_t magic_code);

private:
    void _on_test_net_msg(user_session_ptr user_session, msg_ptr msg);
};
