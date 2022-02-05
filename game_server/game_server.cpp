#include "platform.h"

#include "game_server.h"
#include "ip/address.h"
#include <memory>
#include "package_allocator.hpp"
#include "epoll/acceptor.h"
#include "ip/address.h"
#include "logger/log.h"


net::endpoint endpoint(net::ip::address::from_string("127.0.0.1"), 8000);

game_server::game_server()
	: service_()
	, acceptor_(&service_, endpoint)
{
}

bool game_server::start()
{
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

	return 0;
}