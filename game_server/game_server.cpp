#include "platform.h"

#include "game_server.h"
#include "ip/address.h"
#include <memory>
#include "package_allocator.hpp"
#include "epoll/acceptor.h"
#include "ip/address.h"
#include "logger/log.h"



game_server::game_server()
	: service_()
{
	net::endpoint endpoint(net::ip::address::from_string("127.0.0.1"), 8001);
	acceptor_ = new net::acceptor(&service_, endpoint);
	
	start();
}

bool game_server::start()
{
	_start_nets();
	return true;
}

bool game_server::_start_nets()
{
	service_.start();
	return true;
}

INITIALIZE_EASYLOGGINGPP
int main()
{
	g_init_log(game_server::instance().server_name());

	while (1);
	return 0;
}