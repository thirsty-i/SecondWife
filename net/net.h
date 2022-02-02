#pragma once

#include "platform.h"

#if PLAT_FORM == LINUX
#include "epoll/epoll.h"
using service = net::epoll;
#elif PLAT_FORM == WINDOWS
using test = int;
#endif