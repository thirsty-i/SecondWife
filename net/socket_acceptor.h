#pragma once
#include "asio.hpp"
#include "message_handler.h"
#include "mtl/pool/object_pool.h"
#include "socket_session.h"
#include <set>

class socket_acceptor
{
public:
	using pointer = std::shared_ptr<socket_acceptor>;

	socket_acceptor(asio::io_context& io_context);
	void start(const char* address, uint32_t port);
	void accept();
	void set_message_handler(message_handler* handler);
private:
	asio::ip::tcp::acceptor acceptor_;
	message_handler* handlers_;
	std::set<socket_session::pointer> sessions_;
};