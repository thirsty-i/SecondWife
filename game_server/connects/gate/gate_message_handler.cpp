#include "gate_message_handler.h"
#include "gate_net.h"

gate_message_handler::gate_message_handler(gate_service* service)
    : service_(service)
{

}

void gate_message_handler::on_connected()
{

}

void gate_message_handler::on_recv()
{
    role* role = service_->find_role(10086);
    int msg_id = 10087;

    callbacks_[msg_id](role);
}


void gate_message_handler::on_disconnected()
{

}