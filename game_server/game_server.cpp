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
#include "mtl/struct/ring_buffer.h"

#include "mtl/struct/lock_free/stack.h"

struct node 
	: public mtl::lock_free::stack_node
{
	int a;

	node(int x) : a(x) {};
};

int main()
{
	node* nodes = new node[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	mtl::lock_free::stack stack;

	std::async([&]() {
		while (true)
		{
			for (int i = 0; i < 10; ++i)
			{
				stack.push(nodes + i);
			}
		}
	});
	
	std::async([&]() {
		while (true)
		{
			for (int i = 0; i < 10; ++i)
			{
				stack.pop();
			}
		}
	});


	while (true);

	return 0;
}

