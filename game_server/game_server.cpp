#include "game_server.h"
#include <iostream>

#include "connects/gate/gate_net.h"
#include "net/net.h"

game_server::game_server()
{
	start();
}

bool game_server::start()
{
	gate_net::instance().start();
	net::instance().start();

	while (true)
		;
	return true;
}