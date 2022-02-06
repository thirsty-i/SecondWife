#pragma once
#include "platform.h"
#if PLATFORM == LINUX

#include "pool/object_pool.h"
#include "descriptor_data.h"
#include <thread>
//#include "struct/circle_list.h"
#include "package_allocator.hpp"

namespace net {
class epoll
{
public:
	epoll();
	~epoll();
	std::shared_ptr<descriptor_data> register_descriptor(int descriptor);

	void post_complete_event();
	void start();
private:
	int  _epoll_create();
	void _worker_func();
	void _wait();
private:
	enum { EPOLL_SIZE = 20000 };
	int epoll_fd_;
	bool thread_stop_;
	std::thread* worker_thread_;
	mtl::object_pool<descriptor_data> descriptor_pool_;
	//mtl::circle_list<void*> complete_events_;
};
}; // namespace net
#endif // PLATFORM == LINUX