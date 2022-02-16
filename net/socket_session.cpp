#include "socket_session.h"
#include "common/logger/log.h"

void socket_session::set_message_handler(message_handler* handler)
{
    LOG_PROCESS_ERROR(handler);
    handlers_ = handler;
}