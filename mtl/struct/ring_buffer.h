#pragma once

#include "../common/bitops.h"
#include "../common/noncopyable.h"
#include "memory/memory_resource.h"
#include <assert.h>
#include <cstring>
#include <atomic>
#include <algorithm>

namespace mtl
{
	// single in single out safe
	class ring_buffer final
		: private noncopyable
	{
	public:
		using value_type = char;
		using size_type = size_t;
		using pointer = value_type*;
		using void_pointer = void*;

		ring_buffer(size_type size, const size_type esize = 1)
		{
			LOG_PROCESS_ERROR(size >= 2);
			assert(esize > 0);

			size_t bytes = round_up_pow_of_2(size) * esize;
			_reset((char*)malloc(bytes), bytes);
		}

		~ring_buffer()
		{
			free(buffer_);
		}

		pointer peek_read()
		{
			size_type off = _offset(out_);
			return buffer_ + off * esize_;
		}

		pointer peek_write()
		{
			size_type off = _offset(in_);
			return buffer_ + off * esize_;
		}

		void increment_write(size_t size)
		{
			in_ += (size * esize_);
		}

		void increment_read(size_t size)
		{
			out_ += (size * esize_);
		}

		size_t read_circle_distance()
		{
			return capacity_ - _offset(out_);
		}

		size_t write_circle_distance()
		{
			return capacity_ - _offset(in_);
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
		size_t _offset(size_t index) 
		{
			return index & (capacity_ - 1);
		}

		void _reset(pointer buffer, const size_type size)
		{
			LOG_PROCESS_ERROR(buffer);

			buffer_ = buffer;
			in_ = out_ = 0;
			capacity_ = size;
		}

		void _copy_in(pointer src, size_type len)
		{
			size_type off = _offset(in_);
			size_type size = capacity_;

			if (esize_ != 1) {
				off *= esize_;
				size *= esize_;
				len *= esize_;
			}

			size_type copy_len = std::min<size_type>(len, size - off);

			memcpy(buffer_ + off, src, copy_len);
			memcpy(buffer_, src + copy_len, (len - copy_len));
		}

		void _copy_out(pointer dst, size_type len)
		{
			size_type off = _offset(out_);
			size_type size = capacity_;

			if (esize_ != 1) {
				off *= esize_;
				size *= esize_;
				len *= esize_;
			}

			size_type copy_len = std::min<size_type>(len, size - off);

			memcpy(dst, buffer_ + off, copy_len);
			memcpy(dst + copy_len, buffer_, len - copy_len);
		}

	private:
		alignas(64) size_type in_;
		alignas(64) size_type out_;
		pointer buffer_;
		size_type capacity_;
		size_type esize_;
	};
}