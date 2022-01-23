#if 0
#include "game_server.h"

#include "mtl/struct/tree/map.h"
#include "mtl/thread/spin_lock.hpp"

#include <cstdio>

int main()
{
	typedef typename mtl::map<uint32_t, uint32_t> map_type;
	map_type oMap;

	oMap.insert(std::make_pair(8, 100));
	oMap.insert(std::make_pair(10, 101));

	for (int i = 1; i <= 9; ++i)
	{
		oMap.insert(std::make_pair(9, i));

		printf("print i:%d\n", i);
		oMap.print_tree();
	}

	for (map_type::iterator it = oMap.begin(); it != oMap.end(); ++it)
		std::cout << "key:" << it->first << ",value:" << it->second << std::endl;

	return 0;
}
#endif

#include "mtl/struct/ring_buffer.h"
#include <cstdio>
#include <atomic>
#include "mtl/struct/lock_free/stack.h"

int main()
{
	//#pragma omp parallel for num_threads(2*8-1)
	//	for (int i = 0; i < 1000; ++i)
	//	{
	//		X::get_instance();
	//	}

	

	return 0;
}

