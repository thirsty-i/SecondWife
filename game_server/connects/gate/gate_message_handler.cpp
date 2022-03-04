#include "gate_message_handler.h"
#include "gate_net.h"
#include "common/message/g2t_message.h"
#include "common/message/t2g_message.h"

gate_message_handler::gate_message_handler()
	: message_handler_basic(t2g_message_begin, t2g_message_end)
{
	
}


void gate_message_handler::do_test_net(user_session_t session, int32_t magic_code)
{
	auto msg = std::make_shared<g2c_test_net>();
	msg->magic_code = magic_code;
	session.session->send(msg);
}


void gate_message_handler::_on_test_net_msg(user_session_ptr user_session, msg_ptr msg)
{
	auto msg2 = std::static_pointer_cast<t2g_test_net>(msg);
	
	LOG(DEBUG) << "recv msg:" << msg2->msg_id 
		<< ",magic_code:" << msg2->magic_code;
}