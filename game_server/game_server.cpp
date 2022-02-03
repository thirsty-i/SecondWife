#include "platform.h"

#include "game_server.h"
#include "ip/address.h"

#include "logger/log.h"

game_server::game_server()
	: service_()
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

	using net::ip::address;
	address ress = address::from_string("127.0.0.1");

	LOG(DEBUG) << ress.to_string();
	return 0;
}