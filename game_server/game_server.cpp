#include "game_server.h"
//#include "../include/mtl/map"
//#include "type_int.h"

#include <cstdio>
#include<stdlib.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <atomic>
#include <thread>

typedef void (*pFunc)(void);

volatile int a = 0;
std::atomic<int> b;

void func()
{
	//printf("enter func\n");

	a++;
	b++;
}

void func2()
{
	//printf("enter func2\n");
}


pFunc getFunctionAddress()
{
	int rand = std::rand() % 2;

	if (rand == 0)
		return &func;
	else
		return &func2;
}

std::time_t getTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());//获取当前时间点
	std::time_t timestamp = tp.time_since_epoch().count();
	return timestamp;
}


int main()
{
	//typedef typename mtl::map<UINT32, UINT32> map_type;
	//map_type oMap;
	//oMap.insert(std::make_pair(1, 1));
	//oMap.insert(std::make_pair(2, 1));
	//oMap.insert(std::make_pair(3, 1));
	//oMap.insert(std::make_pair(4, 1));
	//oMap.insert(std::make_pair(5, 1));
	//oMap.insert(std::make_pair(6, 1));

	//for (map_type::iterator iter = oMap.begin(); iter != oMap.end(); ++iter)
	//{
	//	std::cout << "first:" << iter->first << "\t\t\t"
	//		<< "second:" << iter->second << "\n";
	//}
	
	//enum { MAX_LOOP_COUNT = 2147483647 };

	//srand((int)time(0));

	//pFunc pFunction = &func;
	//std::time_t start_time;
	//
	//start_time = getTimeStamp();
	//for (int i = 0; i < MAX_LOOP_COUNT; ++i)
	//	pFunction();
	//std::cout << "indirect call cost:" << getTimeStamp() - start_time << "\n" << std::endl;

	// start_time = getTimeStamp();
	//for(int i = 0; i < MAX_LOOP_COUNT; ++i)
	//	func();
	//std::cout << "direct call cost:" << getTimeStamp() - start_time << "\n" << std::endl;
	std::thread* threads[10];

	for(int i = 0; i < 10; ++i)
		threads[i] = new std::thread(func);


	for (int i = 0; i < 10; ++i)
	{
		threads[i]->join();
		delete threads[i];
	}
	printf("a:%d,b:%d\n", a, b.load());

	return 0;
}