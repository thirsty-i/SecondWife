#include "game_server.h"
#include "net/server.h"
#include "asio.hpp"

#include "mtl/struct/tree/map.h"

game_server::game_server()
{
	start();
}

bool game_server::start()
{
	//LOG(ERROR) << "1111";
	mtl::map<int, int> map;
	map.insert(std::make_pair(10, 10));
	map.insert(std::make_pair(9, 9));
	map.insert(std::make_pair(11, 11));
	map.insert(std::make_pair(8, 8));

	map.print_tree();

	return true;
}