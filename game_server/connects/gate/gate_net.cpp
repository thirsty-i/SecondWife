#include "gate_net.h"
#include "net/net.h"
#include <functional>
#include "common/message/msg_def.h"
#include <memory>
#include "net/buffer.hpp"

gate_net::gate_net()
{
}

//void gate_net::send(user_session_ptr role, std::shared_ptr<msg_header> msg)
//{
//	LOG_PROCESS_ERROR(msg->length);
//	role->session->send(msg);
//}

bool gate_net::start() 
{
	socket_acceptor_ = net::instance().create_acceptor();
	socket_acceptor_->start("127.0.0.1", 8000);
	socket_acceptor_->set_new_connection_handler(std::bind(&gate_net::on_new_connection, this, std::placeholders::_1));

	return true;
}