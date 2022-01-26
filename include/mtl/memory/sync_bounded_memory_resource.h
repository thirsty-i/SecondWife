#pragma once

#include "memory_resource.h"

template <class T>
class sync_bounded_memory_resource
	: private memory_resource
{
public:
	using resource_smart_ptr_t = memory_resource::resource_smart_ptr_t;

	sync_bounded_allocator(size_t size, )
	{
	
	}

	sync_bounded_allocator(void* buffer, size_t buffer_size)
	{
	}

public:
	virtual void* allocate(size_t bytes, size_t alignment) override
	{

	}

	virtual void deallocate(void* ptr, size_t bytes, size_t alignment) override
	{

	}

private:
	allocator
};