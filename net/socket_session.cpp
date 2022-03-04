#include "socket_session.h"
#include "common/logger/log.h"

socket_session::socket_session(asio::io_context& io_context)
	: socket_(io_context)
{

}

void socket_session::send(std::shared_ptr<msg_header> msg)
{
	//socket_.async_send(asio::buffer((void*)msg.get(), msg->length), [msg] {});
}

void socket_session::_recv_handler(msg_base_ptr msg)
{
	if (recv_callback_)
		recv_callback_(msg);
}