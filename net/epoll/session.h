#pragma once

#include "descriptor_data.h"
#include "net.h"
#include "struct/circle_list.h"
#include "msg_header.h"

namespace net {

class session
{
public:
	session(service* service, socket_type s);
	void recv();
	void send(msg_header_t* msg, size_t size);

	using functor_t = std::function<void(session*, char*, size_t)>;
	void set_recv_callback(functor_t f) { recv_handler_ = std::move(f); }
private:

	void _remote_close();
	void _register_callbacks();
	void _handle_read();
	void _handle_write();
	void _handle_except();
private:
	service* service_;
	descriptor_data descriptor_data_;

	mtl::circle_list<msg_header_t*> recv_list_;
	mtl::circle_list<msg_header_t*> send_list_;

	msg_header_t* send_msg_;
	size_t recv_off_;
	size_t send_off_;

	mtl::ring_buffer recv_buffer_;
	functor_t recv_handler_;
};
}