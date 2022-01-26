#pragma once

#include "allocator.h"

template <class T>
class syn_bounded_allocator
	: private allocator<T>
{

	syn_bounded_allocator(size_t size, )

private:
	virtual void* do_allocate(size_t bytes, size_t alignment) override
	{

	}

	virtual void do_deallocate(void* ptr, size_t bytes, size_t alignment) override
	{

	}

private:
	allocator
};