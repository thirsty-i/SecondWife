#include "socket_acceptor.h"
#include "socket_session.h"
#include "net.h"
#include "common/logger/log.h"

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
	acceptor_.async_accept(
		[this](std::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				LOG(ERROR) << "enter aysnc_accept complete call function";
				return;
			}

			socket_session_ptr new_session = net::instance().create_session();

			if(new_connection_callback_)
				new_connection_callback_(new_session);

			_accept();
		});
}