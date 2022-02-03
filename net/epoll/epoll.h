#pragma once
#include "platform.h"
#if PLATFORM == LINUX

#include "pool/object_pool.h"
#include "descriptor.h"
#include <thread>

namespace net {
class epoll
{
public:
	epoll();
	bool register_descriptor(int descriptor);


private:
	int  _epoll_create();
	void _worker_func();
private:
	enum { EPOLL_SIZE = 20000 };
	int epoll_fd_;
	std::thread worker_thread_;
	mtl::object_pool<descriptor> descriptor_pool_;
	bool thread_stop_;
};
}; // namespace net
#endif // PLATFORM == LINUX