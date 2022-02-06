#include "socket_ops.h"
#include "session.h"
#include "bitops.h"

namespace net {

char* session::ring_buffer::peek_read()
{
	size_t off = in_ & (capacity_ - 1);
	return buffer_ + off;
}

char* session::ring_buffer::peek_write()
{
	size_t off = out_ & (capacity_ - 1);
	return buffer_ + off;
}

void session::ring_buffer::incream_write(size_t size)
{
	in_ += size;
}

void session::ring_buffer::incream_read(size_t size)
{
	out_ += size;
}

session::ring_buffer::ring_buffer(size_t size)
	: in_(0)
	, out_(0)
{
	size_t bytes = round_up_pow_of_2(size);
	buffer_ = static_cast<char*>(malloc(bytes));
	capacity_ = bytes;
}

session::ring_buffer::~ring_buffer()
{
	free(buffer_);
}

size_t session::ring_buffer::get_unused() 
{ 
	return capacity_ - (in_ - out_); 
}

size_t session::ring_buffer::get_used() 
{ 
	return in_ - out_; 
}

session::session(service* service, socket_type s)
	: service_(service)
	, send_buffer_(1024)
	, recv_buffer_(1024)
	, socket_(invalid_socket)
	
{
	LOG_PROCESS_ERROR(service_);

	descriptor_data_ = service_->register_descriptor(s);
	LOG_PROCESS_ERROR(descriptor_data_);

	_register_callbacks();
}


void session::_register_callbacks()
{
	descriptor_data_->set_read_callback(std::bind(&session::_handle_read, this));
	descriptor_data_->set_write_callback(std::bind(&session::_handle_write, this));
	descriptor_data_->set_except_callback(std::bind(&session::_handle_except, this));
}

void session::_handle_read()
{
	size_t free_size = recv_buffer_.get_unused();

	if (free_size > 0)
	{
		size_t recv_size = 0;

		recv_size = socket_ops::recv(descriptor_data_->descriptor(), recv_buffer_.peek_write(), free_size, 0);
		if (recv_size < 0)
			return;  // read error

		recv_buffer_.incream_write(recv_size);
	}
}

void session::_handle_write()
{
	size_t used_size = send_buffer_.get_used();
	if (used_size > 0)
	{
		size_t write_size = 0;

		write_size = socket_ops::send(descriptor_data_->descriptor(), send_buffer_.peek_read(), used_size, 0);
		if (write_size < 0)
			return;  // write error

		send_buffer_.incream_read(write_size);
	}
}

void session::_handle_except()
{

}

}