#include "platform.h"
#if PLATFORM == LINUX
#include "epoll.h"
#include <sys/epoll.h>

#include "socket_ops.h"

namespace net {
epoll::epoll()
	: descriptor_pool_(1024)
	, epoll_fd_(_epoll_create())
{
	LOG(DEBUG) << "hello";
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

};
#endif // PLATFORM == LINUX
