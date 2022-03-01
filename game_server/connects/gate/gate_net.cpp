#include "gate_net.h"
#include "net/net.h"

gate_service::gate_service()
    : socket_acceptor_(net::instance().create_acceptor())
{
    socket_acceptor_->set_message_handler(message_handler_);
    socket_acceptor_->set_new_connection_handler([this](socket_session_ptr& new_session) {

        table_.insert(std::make_pair(10086, new role()));
    });
}