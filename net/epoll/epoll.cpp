#include "platform.h"
#if PLATFORM == LINUX
#include "epoll.h"
#include <sys/epoll.h>

#include "socket_ops.h"
#include "logger/log.h"

namespace net {
epoll::epoll()
	: descriptor_pool_(1024)
	//, complete_events_(1024)
	, epoll_fd_(_epoll_create())
	, package_allocator_(1024, 1024)
{
	
}

epoll::~epoll()
{
	thread_stop_ = false;

	if(worker_thread_ && worker_thread_->joinable())
		worker_thread_->join();
}

void epoll::start()
{
	worker_thread_ = new std::thread(&epoll::_worker_func, this);
}

int epoll::_epoll_create()
{
	int fd = epoll_create(EPOLL_SIZE);
	//if (fd != -1)
	//	::fcntl(fd, F_SETFD, FD_CLOEXEC);

	LOG_PROCESS_ERROR_RET(fd != -1, fd);

	return fd;
}

std::shared_ptr<descriptor_data> epoll::register_descriptor(int descriptor)
{
	std::shared_ptr<descriptor_data> data = descriptor_pool_.create();

	epoll_event ev = { 0, { 0 } };
	ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLPRI | EPOLLET;
	data->descriptor() = descriptor;
	ev.data.ptr = data.get();

	int result = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, descriptor, &ev);
	LOG_PROCESS_ERROR_RET(result == 0, NULL);

	return data;
}

void epoll::_worker_func()
{
	LOG(ERROR) << "threadid:" << std::this_thread::get_id();
	while (!thread_stop_)
	{
		_wait();
	}
}

void epoll::_wait()
{
	int number = 0;
	epoll_event epoll_events[128];
	number = epoll_wait(epoll_fd_, epoll_events, 128, -1);

	if (number < 0)
		return;

	LOG(ERROR) << "1111111";
	for (int i = 0; i < number; ++i)
	{
		void* ptr = epoll_events[i].data.ptr;
		uint32_t events = epoll_events[i].events;

		descriptor_data* state = static_cast<descriptor_data*>(ptr);

		if (events & (EPOLLERR))
			state->set_except_op();

		if (events & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
			state->set_read_op();

		if (events & EPOLLOUT)
			state->set_write_op();

		state->perform_io();
	}
}

};
#endif // PLATFORM == LINUX
