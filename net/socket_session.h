#pragma once

#include <memory>
#include "message_handler.h"

class user_session_base;

class socket_session
{
public:
	using recv_callback_t = std::function<void(socket_session_ptr, char*, size_t)>;
	using close_callback_t = std::function<void(socket_session_ptr)>;
public:
	void set_recv_callback(const recv_callback_t& cb)
	{ recv_callback_ = cb; }

	void set_close_callback(const close_callback_t& cb)
	{ close_callback_ = cb; }

	size_t fd() const
	{ return reinterpret_cast<size_t>(this); }

	void fd(size_t fd)
	{  fd_ = fd; }
private:
	recv_callback_t recv_callback_;
	close_callback_t close_callback_;
	size_t fd_;
};

typedef std::shared_ptr<socket_session> socket_session_ptr;