#pragma once
#include <functional>
#include "common/noncopyable.h"
#include "common/message/msg_def.h"
#include "net/user_session.h"
#include "common/logger/log.h"

template <class UserSession, class MsgType>
class message_handler_basic
	: private noncopyable
{
public:
	static_assert(std::is_base_of<user_session_base, UserSession>::value, "UserSession does not inherit user_session_base");
	static_assert(std::is_base_of<msg_header, MsgType>::value, "MsgType does not inherit msg_header");

	using user_session_t = UserSession;
	using msg_t = MsgType;

	using user_session_ptr = std::shared_ptr<user_session_t>;
	using msg_ptr = std::shared_ptr<msg_header>;

	using message_func_t = std::function<void(user_session_ptr, msg_ptr)>;

	explicit message_handler_basic(const size_t msg_begin, const size_t msg_end)
		: msg_begin_(msg_begin)
		, msg_end_(msg_end)
	{
		LOG_PROCESS_ERROR(msg_end - msg_begin > 1);
		recv_callbacks_.resize(msg_end - msg_begin - 1);
	}

	void recv(user_session_ptr user_session, msg_ptr msg)
	{
		LOG_PROCESS_ERROR(msg->msg_id > msg_begin_);
		LOG_PROCESS_ERROR(msg->msg_id < msg_end_);
		LOG_PROCESS_ERROR(recv_callbacks_[msg->msg_id]);

		recv_callbacks_[msg->msg_id](user_session, msg);
	}

	bool register_recv_callback(size_t msg_id, message_func_t&& func)
	{
		LOG_PROCESS_ERROR_RET(msg_id < recv_callbacks_.size(), false);
		LOG_PROCESS_ERROR_RET(!recv_callbacks_[msg_id], false);

		recv_callbacks_[msg_id] = std::move(func);
		return true;
	}
private:
	std::vector<message_func_t> recv_callbacks_;
	size_t msg_begin_;
	size_t msg_end_;
};