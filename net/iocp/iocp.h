#pragma once

#include "platform.h"
#include "descriptor_data.h"
#include <memory>

#if PLATFORM == WINDOWS

namespace net {

class iocp 
{

public:
	void start() {}
	std::shared_ptr<descriptor_data> register_descriptor(int descriptor) { return 0; }
};

}; // namespace net
#endif // PLATFORM == WINDOWS