#pragma once
#include "pool/object_pool.h"
#include "session.h"

namespace net {
class epoll
{
public:
	epoll();
private:
	int _epoll_create();
private:
	enum { EPOLL_SIZE = 20000 };
	int epoll_fd_;
	mtl::object_pool<session> session_pool_;
};
}; // namespace net