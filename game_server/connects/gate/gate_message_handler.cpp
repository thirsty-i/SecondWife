#include "gate_message_handler.h"
#include "gate_net.h"
#include "common/message/g2t_message.h"
#include "common/message/t2g_message.h"
#include <functional>

gate_message_handler::gate_message_handler()
	: message_handler_basic(t2g_message_begin, g2t_message_end)
{
	register_recv_callback((uint32_t)t2g_message_id::t2g_test_net, 
		std::bind(&gate_message_handler::_on_test_net_msg, this, std::placeholders::_1, std::placeholders::_2));
}

// FIXME: Generate code with tools
void gate_message_handler::do_test_net(user_session_ptr session, int32_t magic_code)
{
	buffer_ptr buf = std::make_shared<buffer>(sizeof(g2t_test_net));
	g2t_test_net* msg = reinterpret_cast<g2t_test_net*>(buf->peek_read());
	msg->length = sizeof(g2t_test_net);
	msg->msg_id = (uint32_t)g2t_message_id::g2t_test_net;
	msg->magic_code = magic_code;
	buf->increment_write(msg->length);
	session->session->send(buf);
}

void gate_message_handler::_on_test_net_msg(user_session_ptr user_session, msg_ptr msg)
{
	auto msg2 = static_cast<t2g_test_net*>(msg);
	do_test_net(user_session, 10087);

	printf("msg_id:%d, magic_code:%d\n", msg2->msg_id, msg2->magic_code);
}