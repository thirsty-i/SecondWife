#include "gate_net.h"
#include "net/net.h"
#include <function>

gate_net::gate_net()
    : socket_acceptor_(net::instance().create_acceptor())
{
	socket_acceptor_->set_new_connection_handler(std::bind(&gate_net::_on_new_connection, this, std::placeholders::_1));
}

gate_net::_on_new_connection(socket_session_ptr new_session)
{
	std::shared_ptr<role> role(std::make_shared<role>());
	session_map_.add_map(new_session->fd(), role);

	role->session->set_recv_callback([role](socket_session_ptr, char* buffer, size_t size) {
		message_handler_.on_recv(role, buffer, size);
	});

	role->session->set_close_callback(std::bind(&gate_net::_on_close, this));
}

gate_net::_on_close(socket_session_ptr session)
{
	session_map_.remove_map(session->fd());
}