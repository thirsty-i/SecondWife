#pragma once

#include "asio.hpp"

using asio::ip::address;
using asio::ip::tcp;

namespace net {
class server
{
public:
	server(asio::io_context& io_context, const tcp::endpoint& endpoint);
	void accept();

private:
	tcp::acceptor acceptor_;
};
}