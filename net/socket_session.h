#pragma once

#include <memory>
#include "common/message/msg_def.h"
#include "asio.hpp"
#include "net/buffer.hpp"
#include "mtl/struct/lock_free/queue.h"

class socket_session
	: public std::enable_shared_from_this<socket_session>
{
public:
	typedef std::shared_ptr<socket_session> socket_session_ptr;
	using msg_base_ptr = std::shared_ptr<msg_base>;

	using recv_callback_t = std::function<void(buffer_ptr)>;
	using close_callback_t = std::function<void(socket_session_ptr)>;
public:
	socket_session(asio::io_context& io_context);
	~socket_session();
	void send(buffer_ptr msg);

	void set_recv_callback(const recv_callback_t& cb)
	{ recv_callback_ = cb; }

	void set_close_callback(const close_callback_t& cb)
	{ close_callback_ = cb; }

	size_t fd() const
	{ return fd_; }

	void fd(size_t fd)
	{  fd_ = fd; }

	asio::ip::tcp::socket& socket() { return socket_; }
	void start();
private:
	message_size_t _get_message_size(char* buf);

	void _post_send();
	void _read_message();

	void _close_event();
	void _recv_event(std::unique_ptr<char[]>&& buffer);
	void _read_message_size();
private:
	recv_callback_t recv_callback_;
	close_callback_t close_callback_;
	size_t fd_;
	asio::ip::tcp::socket socket_;

	buffer_ptr recv_buffer_;
	buffer_ptr send_buffer_;
	mtl::queue<buffer_ptr> send_queue_;
};

typedef std::shared_ptr<socket_session> socket_session_ptr;