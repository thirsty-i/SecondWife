#pragma once

#include "descriptor_data.h"
#include "net.h"

namespace net {
	
class session
{
	class ring_buffer
	{
	public:
		char* peek_read();

		char* peek_write();

		void incream_write(size_t size);

		void incream_read(size_t size);

		ring_buffer(size_t size);

		~ring_buffer();
		size_t get_unused();
		size_t get_used();

	private:
		size_t in_;
		size_t out_;
		size_t capacity_;
		char* buffer_;
	};

public:
	session(service* service, socket_type s);
private:
	void _register_callbacks();
	void _handle_read();
	void _handle_write();
	void _handle_except();

private:

	service* service_;
	std::shared_ptr<descriptor_data> descriptor_data_;

	ring_buffer send_buffer_;
	ring_buffer recv_buffer_;
	socket_type socket_;
};
}