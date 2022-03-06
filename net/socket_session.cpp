#include "socket_session.h"
#include "net/net.h"
#include "common/logger/log.h"

socket_session::socket_session(asio::io_context& io_context)
	: socket_(io_context)
	, fd_((size_t)this)
{

}

socket_session::~socket_session()
{
}

void socket_session::send(buffer_ptr msg)
{
	send_queue_.push(msg);

	if (!send_buffer_)
	{
		_post_send();
	}
}

void socket_session::start()
{
	recv_buffer_ = std::make_shared<buffer>();
	_read_message_size();
}

message_size_t socket_session::_get_message_size(char* buf)
{
	message_size_t size = 0;
	return *reinterpret_cast<message_size_t*>(buf);
}

void socket_session::_post_send()
{
	if (!send_queue_.pop(send_buffer_))
		return;

	socket_.async_send(asio::buffer(send_buffer_->peek_read(), send_buffer_->get_used()), 
		[this](const asio::error_code& e, std::size_t)
		{
			if (e)
			{
				LOG(ERROR) << "send error, e:" << e;
				socket_.close();
				net::instance().push_event(std::bind(&socket_session::_close_event, this));
				return;
			}
			
			send_buffer_.reset();
			_post_send();
		});
}

void socket_session::_read_message_size()
{
	recv_buffer_->prepare(MESSAGE_LENGTH_SIZE);
	asio::async_read(socket_, asio::buffer(recv_buffer_->peek_read(), MESSAGE_LENGTH_SIZE),
		[this](const asio::error_code& e, std::size_t bytes_transferred)
		{
			if (e)
			{
				socket_.close();
				net::instance().push_event(std::bind(&socket_session::_close_event, this));
				return;
			}

			if (bytes_transferred == 0)
			{
				_read_message_size();
				return;
			}

			recv_buffer_->increment_write(MESSAGE_LENGTH_SIZE);
			message_size_t message_size = _get_message_size(recv_buffer_->peek_read());

			if (!message_size || message_size > MAX_MESSAGE_SIZE)
			{
				socket_.close();
				net::instance().push_event(std::bind(&socket_session::_close_event, this));
				return;
			}

			_read_message();
		});
}

void socket_session::_read_message()
{
	message_size_t message_size = _get_message_size(recv_buffer_->peek_read());
	recv_buffer_->prepare(message_size);

	asio::async_read(socket_, asio::buffer(recv_buffer_->peek_write(), message_size - MESSAGE_LENGTH_SIZE),
		[this](const asio::error_code& e, std::size_t bytes_transferred)
		{
			if (e)
			{
				socket_.close();
				net::instance().push_event(std::bind(&socket_session::_close_event, this));
				return;
			}

			if (bytes_transferred == 0)
			{
				_read_message();
				return;
			}

			message_size_t message_size = bytes_transferred + MESSAGE_LENGTH_SIZE;
			recv_buffer_->increment_write(bytes_transferred);
			buffer_ptr buf = std::make_shared<buffer>(message_size);
			buf->write(recv_buffer_->peek_read(), message_size);
			recv_buffer_->increment_read(message_size);

			net::instance().push_event([buf, this] {
				if (recv_callback_)
					recv_callback_(buf);
			});

			_read_message_size();
		});
}

void socket_session::_close_event()
{
	if (close_callback_)
		close_callback_(shared_from_this());
}