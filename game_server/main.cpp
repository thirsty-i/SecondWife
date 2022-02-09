#include "common/logger/log.h"
#include "game_server.h"

INITIALIZE_EASYLOGGINGPP
int main()
{
	g_init_log(game_server::instance().server_name());
	return 0;
}