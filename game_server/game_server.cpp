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
#include <iostream>
#include "game_server.h"

//#include "mtl/struct/ring_buffer.h"
#include "common/logger/log.h"
#include "mtl/pool/object_pool.h"
#include "mtl/memory/small_pool_resource.h"

struct test
{
	explicit test(int a) : a(a) {};

	int a;
};

int main()
{
	g_init_log(game_server::instance().server_name());

	//object_pool<A> a_pool(1024);
	//object_pool<B> b_pool(1024);

	//std::shared_ptr<A> a = a_pool.create();
	//std::shared_ptr<B> b = b_pool.create();

	//test* ts = allocator.allocate(1);
	//printf("1. ts:%p, a:%d\n", ts, ts->a);
	//allocator.construct(ts, 10);
	//printf("2. ts:%p, a:%d\n", ts, ts->a);
	//allocator.destroy(ts);
	//printf("3. ts:%p, a:%d\n", ts, ts->a);
	//allocator.deallocate(ts, 1);
	//printf("4. ts:%p, a:%d\n", ts, ts->a);

	//ts = allocator.allocate(1);
	//printf("1. ts:%p, a:%d\n", ts, ts->a);

	//mtl::small_pool_resource small_pool;

	mtl::object_pool<test> pool_a;
	std::shared_ptr<test> shared_a = pool_a.create(10);

	return 0;
}

