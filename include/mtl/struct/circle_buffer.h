#ifndef _H_CIRCLE_BUFFER_H_
#define _H_CIRCLE_BUFFER_H_

#include "allocator.h"
#include "bitops.h"
#include "log.h"
#include "lock.h"
#include <assert.h>

namespace mtl
{
	class ring_buffer final
	{
	public:
		using value_type = char;
		using size_type = size_t;

		using pointer = value_type*;
		using void_pointer = void*;

		ring_buffer()
		{
			reset(0, 0);
		}

		ring_buffer(void_pointer buffer, const size_type size)
			: capacity_(round_up_pow_of_2(size))
		{
			assert(capacity_ >= 2);
			reset(buffer, size);
		}

		~ring_buffer() = default;

		void reset(pointer buffer, const size_type size)
		{
			buffer_ = buffer;
			in_ = out_ = 0;
			capacity_ = size;
		}

		size_type read(pointer dst, size_type len)
		{
			int32_t used = used();
			if (len > used)
				len = used;

			_copy_out(dst, len);
			out_ += len;

			return len;
		}

		size_type write(pointer src, size_type len)
		{
			int32_t unused = unused();
			if (len > unused)
				len = unused;

			_copy_in(src, len);
			in_ += len;

			return len;
		}

		/*Push and Pop*/
		size_type unused() { return capacity_ - (in_ - out_); }
		size_type used() { return in_ - out_; }

		void _copy_in(pointer src, size_type len)
		{
			int32_t off = in_ & (capacity_ - 1);
			int32_t copy_len = min(len, capacity_ - off);

			memcpy(buffer_ + off, src, copy_len);
			memcpy(buffer_, src + copy_len, (len - copy_len));
		}

		void _copy_out(pointer dst, size_type len)
		{
			int32_t off = out_ & (capacity_ - 1);
			int32_t copy_len = min(len, capacity_ - off);

			memcpy(dst, buffer_ + off, copy_len);
			memcpy(dst + copy_len, buffer_, len - copy_len);
		}

	private:
		size_type capacity_; 

		/*modify atomic*/
		alignas(64) size_type in_;
		alignas(64) size_type out_;
		pointer buffer_;
	};

}
#endif // _H_CIRCLE_BUFFER_H_