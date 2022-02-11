#pragma once

#include "asio.hpp"

namespace net {
class server
{
public:
	server(asio::io_context& io_context, const asio::ip::tcp::endpoint& endpoint);
	void accept();

private:
	asio::ip::tcp::acceptor acceptor_;
};
}