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
	map.insert(std::make_pair(100, 100));
	map.insert(std::make_pair(101, 101));
	map.insert(std::make_pair(9, 9));
	map.insert(std::make_pair(20, 20));
	map.insert(std::make_pair(19, 19));
	
	map.erase(map.begin());
	map.erase(map.begin());
	map.erase(map.begin());
	map.erase(map.begin());
	map.erase(map.begin());

	map.print_tree();

	return true;
}