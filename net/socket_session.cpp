#include "socket_session.h"
#include "common/logger/log.h"

void socket_session::set_message_handler(const message_handler_ptr handler)
{
    LOG_PROCESS_ERROR(handler);
    handlers_ = handler;
}