#include "gate_net.h"
#include "net/net.h"
#include <functional>
#include "common/message/msg_def.h"
#include <memory>

gate_net::gate_net()
{
}

void gate_net::_on_new_connection(socket_session_ptr new_session)
{
	user_session_ptr role(new user_session_t());
	role->session = new_session;
	session_map_.add_map(new_session->fd(), role);

	role->session->set_recv_callback([this, role](std::shared_ptr<msg_base> msg) {
		std::shared_ptr<msg_header> gate_msg = std::static_pointer_cast<msg_header>(msg);
		message_handler_.recv(role, gate_msg);
	});

	role->session->set_close_callback(std::bind(&gate_net::_on_close, this, std::placeholders::_1));
}

void gate_net::_on_close(socket_session_ptr session)
{
	session_map_.remove_map(session->fd());
}

void gate_net::send(user_session_ptr role, std::shared_ptr<msg_header> msg)
{
	LOG_PROCESS_ERROR(msg->length);
	role->session->send(msg);
}

void gate_net::start()
{
	socket_acceptor_ = net::instance().create_acceptor();
	socket_acceptor_->start("127.0.0.1", 80000);
	socket_acceptor_->set_new_connection_handler(std::bind(&gate_net::_on_new_connection, this, std::placeholders::_1));
}