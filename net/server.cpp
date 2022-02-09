#include "server.h"
#include <iostream>

using asio::ip::address;
using asio::ip::tcp;

namespace net {

server::server(asio::io_context& io_context, const tcp::endpoint& endpoint)
	//: acceptor_(io_context, endpoint)
{
	accept();
}

void server::accept()
{
	//acceptor_.async_accept(
	//	[this](std::error_code ec, tcp::socket socket)
	//	{
	//		if (!ec)
	//		{
	//			std::cout << "enter aysnc_accept complete call function" << std::endl;
	//		}

	//		char buffer[10] = "hello\n";
	//		asio::async_write(socket, asio::buffer(buffer, sizeof(buffer)), [](const asio::error_code& e, std::size_t) {

	//			});

	//		accept();
	//	});
}

}