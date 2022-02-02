#pragma once
#include "platform.h"

#if  PLATFORM == WINDOWS
using socket_type = SOCKET;
#elif PLATFORM == LINUX
using socket_type = int;
#endif