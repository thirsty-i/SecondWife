#include "game_server.h"
#include "common/logger/log.h"

INITIALIZE_EASYLOGGINGPP
int main()
{
	g_init_log(game_server::instance().server_name());
	game_server::instance().start();
	return 0;
}