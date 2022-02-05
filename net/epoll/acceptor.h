#pragma once

#include "endpoint.h"
#include "socket_ops.h"
#include "descriptor_data.h"
#include "net.h"
#include "session.h"
#include "logger/log.h"

namespace net {
class acceptor 
{
public:
	explicit acceptor(service* service, endpoint& endpoint)
		: service_(service) 
		, socket_(invalid_socket)
	{
		int result = 0;
		LOG_PROCESS_ERROR(service_);
		// TODO£º Scalable
		socket_ = socket_ops::open(endpoint.family(), SOCK_STREAM, IPPROTO_TCP);
		LOG_PROCESS_ERROR(socket_ != invalid_socket);

		result = socket_ops::set_non_block(socket_, 1);
		LOG_PROCESS_ERROR(result == 0);

		result = socket_ops::bind(socket_, endpoint.data(), endpoint.size());
		LOG_PROCESS_ERROR(result == 0);

		result = socket_ops::listen(socket_, SOMAXCONN);
		LOG_PROCESS_ERROR(result == 0);

		descriptor_data_ = service_->register_descriptor(socket_);
		LOG_PROCESS_ERROR(descriptor_data_);

		_register_callbacks();
	}

private:
	void _register_callbacks()
	{
		descriptor_data_->set_read_callback(std::bind(&acceptor::_handle_read, this));
	}

	void _handle_read()
	{
		// ÔÝÊ±²»¼ÇÂ¼remote address
		socket_type remote = invalid_socket;
		remote = socket_ops::accept(socket_, 0, 0);

		if (remote < 0)
			return;

		session* remote_session = new session(service_);

		service_->register_descriptor(remote);

		LOG(ERROR) << "hello, remote:" << remote
			<< "you leak...";
	}

private:
	std::shared_ptr<descriptor_data> descriptor_data_;
	socket_type socket_;
	service* service_;
};
}