#pragma once

#include "platform.h"

#if PLATFORM == LINUX
#include "epoll/epoll.h"
using service = net::epoll;
#elif PLATFORM == WINDOWS
#include "iocp/iocp.h"
using service = net::iocp;
#endif