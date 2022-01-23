#pragma once

#include "common/bitops.h"
#include "common/noncopyable.h"
#include <assert.h>
#include <cstring>
#include <atomic>
#include <algorithm>

namespace mtl
{
	class ring_buffer
		: private noncopyable
	{
	public:
		using value_type = char;
		using size_type = size_t;

		using pointer = value_type*;
		using void_pointer = void*;

		explicit ring_buffer(pointer buffer, const size_type size)
		{
			reset(buffer, round_down_pow_of_2(size));
		}

		void reset(pointer buffer, const size_type size)
		{
			assert(size >= 2);
			assert(buffer);

			buffer_ = buffer;
			in_ = out_ = 0;
			capacity_ = size;
		}

		size_type read(pointer dst, size_type len)
		{
			len = std::min<size_type>(len, get_used());

			_copy_out(dst, len);
			out_ += len;
			std::atomic_thread_fence(std::memory_order_release);

			return len;
		}

		size_type write(pointer src, size_type len)
		{
			len = std::min<size_type>(len, get_unused());

			_copy_in(src, len);
			in_ += len;
			std::atomic_thread_fence(std::memory_order_release);

			return len;
		}

		size_type get_unused() { return capacity_ - (in_ - out_); }
		size_type get_used() { return in_ - out_; }

	private:
		void _copy_in(pointer src, size_type len)
		{
			size_type off = in_ & (capacity_ - 1);
			size_type copy_len = std::min<size_type>(len, capacity_ - off);

			memcpy(buffer_ + off, src, copy_len);
			memcpy(buffer_, src + copy_len, (len - copy_len));
		}

		void _copy_out(pointer dst, size_type len)
		{
			size_type off = out_ & (capacity_ - 1);
			size_type copy_len = std::min<size_type>(len, capacity_ - off);

			memcpy(dst, buffer_ + off, copy_len);
			memcpy(dst + copy_len, buffer_, len - copy_len);
		}

	private:
		alignas(64) size_type in_;
		alignas(64) size_type out_;
		pointer buffer_;
		size_type capacity_;
	};
}