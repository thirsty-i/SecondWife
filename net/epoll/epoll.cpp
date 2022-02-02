#include "epoll.h"
#include <sys/epoll.h>

namespace net {
epoll::epoll()
	: session_pool_(1024)
	, epoll_fd_(_epoll_create())
{
	
}

int epoll::_epoll_create()
{
	int fd = epoll_create(EPOLL_SIZE);
	//if (fd != -1)
	//	::fcntl(fd, F_SETFD, FD_CLOEXEC);

	LOG_PROCESS_ERROR_RET(fd != -1, fd);

	return fd;
}
};