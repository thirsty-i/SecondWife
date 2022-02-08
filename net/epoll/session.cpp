#include "socket_ops.h"
#include "session.h"
#include "bitops.h"
#include "msg_header.h"
#include "cassert"

namespace net {
session::session(service* service, socket_type s)
	: service_(service)
	, recv_buffer_(MAX_MSG_SIZE)
	, recv_off_(0)
	, send_off_(0)
	, recv_list_(1024) // FIXME: configure instead
	, send_list_(1024) // FIXME: configure instead
{
	LOG_PROCESS_ERROR(service_);
	LOG_PROCESS_ERROR(s != invalid_socket);

	descriptor_data_.descriptor() = s;

	int result = socket_ops::set_non_block(descriptor_data_.descriptor(), 1);
	LOG_PROCESS_ERROR(result == 0);

	result = service_->register_descriptor(descriptor_data_);
	LOG_PROCESS_ERROR(result);

	_register_callbacks();
}

void session::send(msg_header_t* msg, size_t size)
{
	LOG_PROCESS_ERROR(!send_list_.full());
	LOG_PROCESS_ERROR(!size);
	LOG_PROCESS_ERROR(!msg);

	//size_t bytes = size + sizeof(msg_header_t);
	//msg_header_t* msg = static_cast<msg_header_t*>(malloc(bytes));

	send_list_.push(msg);
}

void session::close()
{
	service_->unregister_descriptor();
}

void session::_remote_close()
{
	service_->unregister_descriptor();

	// to main thread queue
}

void session::recv()
{
	LOG_PROCESS_ERROR(!recv_list_.empty());
	LOG_PROCESS_ERROR(recv_handler_);

	msg_header_t* msg;
	recv_list_.pop(msg);

	recv_handler_(this, msg->data, msg->msg_size);
	free(msg);
}

void session::_register_callbacks()
{
	descriptor_data_.set_read_callback(std::bind(&session::_handle_read, this));
	descriptor_data_.set_write_callback(std::bind(&session::_handle_write, this));
	descriptor_data_.set_except_callback(std::bind(&session::_handle_except, this));
}

void session::_handle_read()
{
	if (recv_list_.full())
		return; // waiting for consumption

	size_t expect_size = std::min<size_t>(recv_buffer_.get_unused(), recv_buffer_.write_circle_distance());
	/* FIXME:
	*  When recv_buffer_ wraps around, expect_size may be small. Optimize for performance bottlenecks
	*/
	if (expect_size > 0)
	{
		size_t actual_size = 0;
		actual_size = socket_ops::recv(descriptor_data_.descriptor(), recv_buffer_.peek_write(), expect_size, 0);
		if (actual_size < 0)
			return _remote_close();

		recv_buffer_.increment_write(actual_size);
	}

	size_t used_bytes = recv_buffer_.get_used();
	if (used_bytes > sizeof(msg_header_t))
	{
		msg_header_t* header = reinterpret_cast<msg_header_t*>(recv_buffer_.peek_read());
		size_t msg_size = header->msg_size + sizeof(header);
		if (used_bytes >= msg_size)
		{
			header = static_cast<msg_header_t*>(malloc(msg_size));
			recv_buffer_.read((char*)header, msg_size);
			recv_list_.push(header);
			// send recv event to service
		}
	}
}

void session::_handle_write()
{
	//size_t expect_size = std::min<size_t>(send_buffer_.get_used(), send_buffer_.read_circle_distance());
	//if (expect_size > 0)
	//{
	//	size_t actual_size = 0;
	//	actual_size = socket_ops::recv(descriptor_data_.descriptor(), send_buffer_.peek_read(), expect_size, 0);
	//	if (actual_size < 0)
	//		return;  // read error
	//	recv_buffer_.increment_write(actual_size);
	//}

	if (!send_msg_)
	{
		send_list_.pop(send_msg_);
		if (!send_msg_)
			return;  // remove write descriptor 
	}

	//FIXME: optimization: expect_size is too small send later???
	size_t expect_size = send_msg_->msg_size - send_off_;
	LOG_PROCESS_ERROR(expect_size);

	size_t actual_size = 0;
	actual_size = socket_ops::send(descriptor_data_.descriptor(), (void*)send_msg_, expect_size, 0);

	if (actual_size < 0)
		return _remote_close();

	send_off_ += actual_size;
	if (send_off_ == send_msg_->msg_size)
	{
		free(send_msg_);
		send_off_ = 0;
		send_msg_ = 0;
	}
}

void session::_handle_except()
{

}

}