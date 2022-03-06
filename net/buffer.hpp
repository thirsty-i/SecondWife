#pragma once

#include <cassert>
#include "common/logger/log.h"

class buffer final
{
public:
	buffer(size_t size = 128)
		: capacity_(size)
		, in_(0)
		, out_(0)
		, buffer_(0)
	{
		assert(capacity_);
		buffer_.reset(new char[size]);
		printf("[ctor] this:%p, thread_id:%d\n", this, std::this_thread::get_id());
	}

	buffer(buffer&& other)
		: capacity_(std::exchange(other.capacity_, 0))
		, in_(std::exchange(other.in_, 0))
		, out_(std::exchange(other.out_, 0))
		, buffer_(std::move(other.buffer_))
	{
		printf("[ctor] this:%p, thread_id:%d\n", this, std::this_thread::get_id());
	}

	~buffer()
	{
		printf("[dtor] this:%p, thread_id:%d\n", this, std::this_thread::get_id());
	}

	bool read(void* p, size_t size)
	{
		LOG_PROCESS_ERROR_RET(p, false);
		LOG_PROCESS_ERROR_RET(size, false);
		LOG_PROCESS_ERROR_RET(get_unused() >= size, false);

		memcpy(p, peek_read(), size);
		increment_read(size);
	}

	void prepare(size_t size)
	{
		if (capacity_ - out_ > size)
			return;

		size_t unused = get_unused();
		size_t used = get_used();

		if (unused < size)
		{
			char* buffer = new char[size + capacity_];
			memcpy(buffer, peek_read(), used);
			buffer_.reset(buffer);
			in_ = used;
			out_ = 0;
			return;
		}

		memmove(buffer_.get(), peek_read(), used);
		in_ = used;
		out_ = 0;
	}

	void write(void* p, size_t size)
	{
		prepare(size);
		memcpy(peek_write(), p, size);
		increment_write(size);
	}

	char* peek_write()
	{
		return buffer_.get() + in_;
	}

	char* peek_read()
	{
		return buffer_.get() + out_;
	}

	void increment_write(size_t size)
	{
		in_ += size;
	}

	void increment_read(size_t size)
	{
		out_ += size ;
	}

	size_t get_unused() { return capacity_ - (in_ - out_); }
	size_t get_used() { return in_ - out_; }
	
private:
	std::unique_ptr<char[]> buffer_;
	size_t in_;
	size_t out_;
	size_t capacity_;
};

using buffer_ptr = std::shared_ptr<buffer>;