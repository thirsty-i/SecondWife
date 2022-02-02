#include "game_server.h"
#include "logger/log.h"
#include "platform.h"

game_server::game_server()
	: service_()
{
#if PLAT_FORM == LINUX
	LOG(DEBUG) << "11111";
#elif PLAT_FORM == WINDOWS
	LOG(DEBUG) << "2222";
#endif
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