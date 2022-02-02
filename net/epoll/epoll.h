#pragma once
#include "platform.h"
#if PLATFORM == LINUX

#include "pool/object_pool.h"
#include "descriptor.h"

namespace net {
class epoll
{
public:
	epoll();
	bool register_descriptor(int descriptor);
	void set_non_block(int descriptor);

private:
	int _epoll_create();
private:
	enum { EPOLL_SIZE = 20000 };
	int epoll_fd_;
	mtl::object_pool<descriptor> descriptor_pool_;
};
}; // namespace net
#endif // PLATFORM == LINUX