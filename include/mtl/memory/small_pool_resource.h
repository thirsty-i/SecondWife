#pragma once

#include "mtl/pool/chunk_pool.h"
#include "allocator.h"
#include "memory_resource.h"

namespace mtl {
class small_pool_resource final
	: public  memory_resource
	, private noncopyable
{
public:
	small_pool_resource(memory_resource* resource = get_new_delete_resource()) 
		: allocator_(resource)
	{
		LOG_PROCESS_ERROR(resource);

		pools_ = allocator_.allocate(SMALL_POOLS);
		LOG_PROCESS_ERROR(pools_);

		for (size_t i = 0; i < SMALL_POOLS; ++i)
		{
			size_t block_size = ALIGN + i * ALIGN;
			allocator_.construct(pools_ + i, block_size, (size_t)4096 / block_size); // 4096替换为一页大小
		}
	}

	small_pool_resource& operator=(small_pool_resource&&) noexcept = default;
	small_pool_resource(small_pool_resource&&) noexcept = default;

	~small_pool_resource()
	{
		for (size_t i = 0; i < SMALL_POOLS; ++i)
		{
			allocator_.destroy(pools_ + i);
		}
		allocator_.deallocate(pools_, SMALL_POOLS);
	}

	virtual void* do_allocate(const size_t bytes, const size_t align) override
	{
		if (bytes > MAX_BYTES)
			return 0;

		size_t index = _get_index(bytes);
		return pools_[index].allocate();
	}

	virtual void do_deallocate(void* ptr, const size_t bytes, const size_t align) override
	{
		if (bytes > MAX_BYTES)
			return;

		size_t index = _get_index(bytes);
		pools_[index].deallocate(ptr);
	}

private:
	size_t _get_index(size_t bytes)
	{
		return (((bytes)+(size_t)ALIGN - 1) / (size_t)ALIGN - 1);
	}

	enum { ALIGN = alignof(void*) };
	enum { MAX_BYTES = 128 };
	enum { SMALL_POOLS = MAX_BYTES / ALIGN };

	using pool_t = mtl::chunk_pool;

	pool_t* pools_;
	mtl::allocator<pool_t> allocator_;
};
};