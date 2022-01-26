#pragma once

#include <memory>
#include "common/noncopyable.h"
#include "memory_resource.h"
#include "common/logger/log.h"

template <class T>
class allocator : private noncopyable
{
public:
	using value_t = T;
	using pointer_t = T*;
	using resource_smart_ptr_t = memory_resource::resource_smart_ptr_t;

	// TODO: 添加默认参数
	allocator(resource_smart_ptr_t&& resource) : resource_(resource) 
	{
		LOG_PROCESS_ERROR_RET(resource_)
	};

	pointer_t allocate(size_t count)
	{
		void* const p = resource_->allocate(count * sizeof(value_t), alignof(value_t));
		return static_cast<pointer_t>(p);
	}

	void deallocate(value_t* const ptr, size_t count)
	{
		resource_->deallocate(ptr, sizeof(value_t) * count, alignof(value_t));
	}

private:
	resource_smart_ptr_t resource_;
};