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

#include <cstdio>
#include <atomic>
#include <thread>
#include <future>
#include <iostream>
#include "mtl/struct/ring_buffer.h"
#include "mtl/pool/unbounded_object_pool.h"
#include "mtl/pool/bounded_object_pool.h"
#include "common/logger/log.h"

struct test
{
	int a;
};

int main()
{
	g_init_log("game_server");

	//mtl::unbounded_object_pool<test, 1> aaa;
	//{
	//	std::shared_ptr<test> bbb = aaa.allocate();

	//	std::shared_ptr<test> ccc = aaa.allocate();
	//	int a = 10;
	//}
	
	mtl::bounded_object_pool<test, 1> aaa;
	{
		std::shared_ptr<test> bbb = aaa.allocate();
		std::shared_ptr<test> ccc = aaa.allocate();

		LOG_CHECK_ERROR(bbb);
		LOG_CHECK_ERROR(ccc);
		ccc->a = 10;
	}

	return 0;
}

