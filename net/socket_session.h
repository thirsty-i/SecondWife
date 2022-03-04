#pragma once

#include <memory>
#include "common/message/msg_def.h"
#include "asio.hpp"

class socket_session
{
public:
	typedef std::shared_ptr<socket_session> socket_session_ptr;
	using msg_base_ptr = std::shared_ptr<msg_base>;

	using recv_callback_t = std::function<void(msg_base_ptr)>;
	using close_callback_t = std::function<void(socket_session_ptr)>;
public:
	socket_session(asio::io_context& io_context);
	void send(std::shared_ptr<msg_header> msg);

	void set_recv_callback(const recv_callback_t& cb)
	{ recv_callback_ = cb; }

	void set_close_callback(const close_callback_t& cb)
	{ close_callback_ = cb; }

	size_t fd() const
	{ return reinterpret_cast<size_t>(this); }

	void fd(size_t fd)
	{  fd_ = fd; }

	asio::ip::tcp::socket& socket() { return socket_; }
private:
	void _recv_handler(msg_base_ptr msg);
private:
	recv_callback_t recv_callback_;
	close_callback_t close_callback_;
	size_t fd_;
	asio::ip::tcp::socket socket_;
	
};

typedef std::shared_ptr<socket_session> socket_session_ptr;