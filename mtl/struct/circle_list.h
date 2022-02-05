#pragma once

#include "struct/ring_buffer.h"

namespace mtl
{
	template <class T>
	class circle_list final
	{
	public:
		// If size is not a power of 2, it will expand
		explicit circle_list(const size_t size, memory_resource* resource = get_new_delete_resource())
			: ring_buffer_(size, sizeof(T))
		{};

		bool push(const T& data)
		{
			if (full())
				return false;

			ring_buffer_.write(data, 1);
		}

		bool pop(T& data)
		{
			if (empty())
				return false;

			ring_buffer_.read(data, 1);
			return true;
		}

		bool empty() { return !ring_buffer_.get_used(); }
		bool full() { return !ring_buffer_.get_unused(); }
	private:
		ring_buffer ring_buffer_;
	};
};