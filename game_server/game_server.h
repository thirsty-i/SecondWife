#pragma once

#include "mtl/design_pattern/singleton.h"
#include "net/net.h"

class game_server 
	: public mtl::singleton<game_server>
{
public:
	game_server();

	bool main_loop();
	inline const char* server_name() { return "game_server"; }
	bool start();
private:
private:
	//service service_;
	friend class mtl::singleton<game_server>;
};
