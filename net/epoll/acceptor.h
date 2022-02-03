#pragma once

#include "endpoint.h"
#include "socket_ops.h"
#include "logger/log.h"

namespace net {
template <class Service>
class accept
{
public:
	accept(Service* service, const endpoint& endpoint)
		: service_(service) 
		, socket_(invalid_socket)
	{
		int result = 0;
		LOG_PROCESS_ERROR(service_);
		// TODO£º Scalable
		socket_ = socket_ops::open(endpoint.family(), SOCK_STREAM, IPPROTO_TCP);
		LOG_PROCESS_ERROR(socket_ != invalid_socket);

		result = socket_ops::set_non_block(socket_, 1);
		LOG_PROCESS_ERROR(result);

		result = socket_ops::bind(socket_, endpoint.data(), endpoint.size());
		LOG_PROCESS_ERROR(result);

		result = socket_ops::listen(socket_, SOMAXCONN);
		LOG_PROCESS_ERROR(result);
	}

private:

	socket_type socket_;
	Service* service_;
};
}