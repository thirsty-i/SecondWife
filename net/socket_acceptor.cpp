#include "socket_acceptor.h"
#include <iostream>
#include "common/logger/log.h"
#include "socket_session.h"
#include "net.h"

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
	accept();
}

void socket_acceptor::set_message_handler(message_handler* handler)
{
	LOG_PROCESS_ERROR(handler);
	handlers_ = handler;
}

void socket_acceptor::accept()
{
	acceptor_.async_accept(
		[this](std::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "enter aysnc_accept complete call function" << std::endl;
				return;
			}

			socket_session::pointer new_session = net::instance().create_session();
			new_session->set_message_handler(handlers_);
			sessions_.insert(new_session);

			accept();
		});
}