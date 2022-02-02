#pragma once

#include "design_pattern/singleton.h"
#include "net.h"

class game_server 
	: public mtl::singleton<game_server>
{
public:
	game_server();

	bool main_loop();
	inline const char* server_name() { return "game_server"; }

	bool start();
private:
	bool _start_nets();
private:
	service service_;
	friend class mtl::singleton<game_server>;
};
