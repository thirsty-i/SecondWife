#pragma once

#include "mtl/design_pattern/singleton.h"

class game_server : public mtl::singleton<game_server>
{
public:
	bool main_loop();
	
	
	inline const char* server_name() { return "game_server"; }


private:

private:

	friend class mtl::singleton<game_server>;
};
