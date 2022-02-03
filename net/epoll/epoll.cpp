#include "platform.h"
#if PLATFORM == LINUX
#include "epoll.h"
#include <sys/epoll.h>

#include "socket_ops.h"

namespace net {
epoll::epoll()
	: descriptor_pool_(1024)
	, epoll_fd_(_epoll_create())
	, worker_thread_(&_worker_func, this)
{
	
}

epoll::~epoll()
{
	thread_stop_ = false;
	worker_thread_.join();
}

int epoll::_epoll_create()
{
	int fd = epoll_create(EPOLL_SIZE);
	//if (fd != -1)
	//	::fcntl(fd, F_SETFD, FD_CLOEXEC);

	LOG_PROCESS_ERROR_RET(fd != -1, fd);

	return fd;
}

bool epoll::register_descriptor(int descriptor)
{


	return true;
}

void epoll::_worker_func()
{
	int number = 0;
	epoll_event events[128];

	while (!thread_stop_)
	{
		number = epoll_wait(epoll_fd_, events, 128, -1);

	}
}

};
#endif // PLATFORM == LINUX
