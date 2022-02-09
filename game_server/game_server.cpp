#include "platform.h"
#include "game_server.h"
#include "logger/log.h"
#include "server.h"

game_server::game_server()
{
	start();
}

bool game_server::start()
{
	_start_nets();
	return true;
}

bool game_server::_start_nets()
{
	return true;
}

INITIALIZE_EASYLOGGINGPP
int main()
{
	g_init_log(game_server::instance().server_name());
	
	asio::io_context io_context(0);
	tcp::endpoint endpoint(address::from_string("127.0.0.1"), 8080);
	net::server(io_context, endpoint);

	return 0;
}