#pragma once
#include "asio.hpp"
#include "message_handler.h"
#include "mtl/pool/object_pool.h"
#include "socket_session.h"

class socket_acceptor
{
public:
	using new_connection_callback_t = std::function<void(socket_session_ptr)>;

	socket_acceptor(asio::io_context& io_context);
	void start(const char* address, uint32_t port);
	void set_new_connection_handler(const new_connection_callback_t& cb)
	{ new_connection_callback_ = std::move(cb); }
	
private:
	void _accept();
private:
	asio::ip::tcp::acceptor acceptor_;
	new_connection_callback_t new_connection_callback_;
};

using socket_acceptor_ptr = std::shared_ptr<socket_acceptor>;