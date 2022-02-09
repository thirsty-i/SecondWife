#include "game_server.h"
#include "net/server.h"

#include "../deps/asio.hpp"
#include "../deps/asio/io_context.hpp"

game_server::game_server()
{
	start();
}

bool game_server::start()
{
	_start_nets();
	return true;
}

using asio::ip::address;
using asio::ip::tcp;

bool game_server::_start_nets()
{
	asio::io_context io_context(0);
	tcp::endpoint endpoint(address::from_string("127.0.0.1"), 8080);
	net::server server(io_context, endpoint);

	return true;
}

