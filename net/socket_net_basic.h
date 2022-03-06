#pragma once
#include "session_map.h"

template <class UserSession, class MessageHandler>
class socket_net_basic
{
protected:
    using user_session_t = UserSession;
    using session_map_t = session_map<size_t, user_session_t>;
    using user_session_ptr = std::shared_ptr<user_session_t>;
    using message_handler_t = MessageHandler;
    using self_t = socket_net_basic<user_session_t, message_handler_t>;
public:
    const message_handler_t& message_handler() const
    { 
        return message_handler_; 
    }

    const session_map_t& get_session_map() const
    {
        return session_map_;
    }

	virtual bool start() = 0;
protected:
    virtual void on_new_connection(socket_session_ptr new_session)
    {
		user_session_ptr user_session(new user_session_t());
        user_session->session = new_session;
		session_map_.add_map(new_session->fd(), user_session);

		LOG(DEBUG) << "new_session:" << new_session->fd();

        new_session->set_recv_callback([this, user_session](buffer_ptr msg) {
			msg_header* gate_msg = reinterpret_cast<msg_header*>(msg->peek_read());
			message_handler_.recv(user_session, gate_msg);
		});

        new_session->set_close_callback(std::bind(&self_t::on_close, this, std::placeholders::_1));
        new_session->start();
    }

	virtual void on_close(socket_session_ptr session)
	{
		LOG(DEBUG) << "close_session:" << session->fd();
		user_session_ptr user_session = session_map_.find(session->fd());
		user_session->session.reset();

		session_map_.remove_map(session->fd());
	}

protected:
    session_map_t session_map_;
    message_handler_t message_handler_;
};