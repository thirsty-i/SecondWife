#pragma once 

#include <cstddef>
#include "common/bitops.h"
#include "common/logger/log.h"
#include "common/noncopyable.h"

class memory_resource
{
public:
	static constexpr size_t max_align = alignof(max_align_t);
	virtual ~memory_resource() noexcept = default;

	void* allocate(size_t bytes, const size_t align = max_align)
	{
		LOG_PROCESS_ERROR_RET(bytes, 0);
		LOG_PROCESS_ERROR_RET(is_2_pow(align), 0);

		return do_allocate(bytes, align);
	}

	void deallocate(void* ptr, size_t bytes, const size_t align = max_align)
	{
		LOG_PROCESS_ERROR(ptr);
		LOG_PROCESS_ERROR(is_2_pow(align));
		LOG_PROCESS_ERROR(bytes);

		do_deallocate(ptr, bytes, align);
	}

protected:
	virtual void* do_allocate(const size_t bytes, const size_t align) = 0;
	virtual void  do_deallocate(void* ptr, const size_t bytes, const size_t align) = 0;
};

class new_delete_memory_resource 
	: public memory_resource
	, private noncopyable
{
public:
	virtual ~new_delete_memory_resource() noexcept = default;

protected:

	virtual void* do_allocate(const size_t bytes, const size_t align) override
	{
		return ::operator new(bytes);
	}

	virtual void do_deallocate(void* ptr, const size_t bytes, const size_t align) override
	{
		::operator delete(ptr, bytes);
	}
};

inline memory_resource*
get_new_delete_resource()
{
	static new_delete_memory_resource resource;
	return static_cast<memory_resource*>(&resource);
}

static size_t round_up(const size_t bytes, const size_t align)
{
	return (((bytes)+(size_t)align - 1) & ~((size_t)align - 1));
}