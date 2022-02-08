#pragma once
#include "platform.h"
#if PLATFORM == LINUX

#include "descriptor_data.h"
#include <thread>
#include "struct/lock_free/queue.h"

namespace net {
class epoll
{
public:
	epoll();
	~epoll();
	bool register_descriptor(descriptor_data& data);
	bool unregister_descriptor(descriptor_data& data);

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
	mtl::queue<>
};
}; // namespace net
#endif // PLATFORM == LINUX