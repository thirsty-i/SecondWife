#pragma once

#include "basic_endpoint.hpp"

namespace net {
namespace ip{
	class tcp
	{
	public:
		using endpoint = basic_endpoint<tcp>;
	};
}
}