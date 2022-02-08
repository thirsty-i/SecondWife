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
	{
		int result = 0;
		LOG_PROCESS_ERROR(service_);
		// TODO£º Scalable
		socket_type socket = socket_ops::socket(endpoint.family(), SOCK_STREAM, IPPROTO_TCP);
		LOG_PROCESS_ERROR(socket != invalid_socket);

		result = socket_ops::set_non_block(socket, 1);
		LOG_PROCESS_ERROR(result == 0);

		result = socket_ops::bind(socket, endpoint.data(), endpoint.size());
		LOG_PROCESS_ERROR(result == 0);

		result = socket_ops::listen(socket, SOMAXCONN);
		LOG_PROCESS_ERROR(result == 0);

		descriptor_data_.descriptor() = socket;
		result = service_->register_descriptor(descriptor_data_);
		LOG_PROCESS_ERROR(result);

		_register_callbacks();
	}
private:
	void _register_callbacks()
	{
		descriptor_data_.set_read_callback([this]() { acceptor::_handle_read(); });
	}

	void _handle_read()
	{
		// ÔÝÊ±²»¼ÇÂ¼remote address
		socket_type remote = invalid_socket;
		remote = socket_ops::accept(descriptor_data_.descriptor(), 0, 0);

		if (remote < 0)
			return;

		session* remote_session = new session(service_, remote);

		LOG(ERROR) << "hello, remote:" << remote
			<< "you leak...";
	}
private:
	descriptor_data descriptor_data_;
	service* service_;
};
}