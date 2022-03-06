#include "socket_acceptor.h"
#include "socket_session.h"
#include "net.h"
#include "common/logger/log.h"
#include "asio.hpp"

using asio::ip::address;
using asio::ip::tcp;

socket_acceptor::socket_acceptor(asio::io_context& io_context)
	: acceptor_(io_context)
{
}

void socket_acceptor::start(const char* address, uint32_t port)
{
	tcp::endpoint endpoint(address::from_string(address), port);
	acceptor_.open(endpoint.protocol());
	acceptor_.bind(endpoint);
	acceptor_.listen();
	_accept();
}

void socket_acceptor::_accept()
{
	socket_session_ptr new_session = net::instance().create_session();
	acceptor_.async_accept(new_session->socket(), std::bind(&socket_acceptor::_accept_handler, this, new_session, std::placeholders::_1));
}

void socket_acceptor::_accept_handler(socket_session_ptr new_session, std::error_code ec)
{
	LOG_PROCESS_ERROR(!ec);

	net::instance().push_event([new_session, this] {
		if (new_connection_callback_)
			new_connection_callback_(new_session);
	});

	_accept();
}