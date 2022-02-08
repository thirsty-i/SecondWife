#pragma once
#include "struct/ring_buffer.h"

namespace mtl
{
	template <class T>
	class circle_list final
	{
	public:
		// If size is not a power of 2, it will expand
		circle_list(const size_t size)
			: ring_buffer_(size, sizeof(T))
		{}

		~circle_list() {}

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

		T front()
		{
			if (empty())
				return T();

			return ring_buffer_.peek_read();
		}

		bool empty() { return !ring_buffer_.get_used(); }
		bool full() { return !ring_buffer_.get_unused(); }
	private:
		ring_buffer ring_buffer_;
	};
};