#pragma once

#include "mtl/memory/memory_resource.h"
#include "mtl/memory/allocator.h"
#include "mtl/memory/chunk.h"
#include "chunk_pool.h"
#include <type_traits>

namespace mtl {
template<class T>
class object_pool
{
public:
	object_pool(size_t size = DEFAULT_SIZE, memory_resource* resource = get_new_delete_resource())
		: allocator_(resource)
		, init_size_(size)
		, chunk_pool_(sizeof(T), init_size_, 50)
	{
		LOG_PROCESS_ERROR(resource);
		LOG_PROCESS_ERROR(init_size_);
	}

	template<class ...Args>
	std::shared_ptr<T> create(Args&&... args)
	{
		static auto deleter = [&](T* ptr) {
			release(ptr);
		};

		std::shared_ptr<T> res((T*)chunk_pool_.allocate(), deleter);

		LOG_PROCESS_ERROR_RET(res, res);

		allocator_.construct(res.get(), std::forward<Args>(args)...);

		return res;
	}

	void release(T* ptr)
	{
		allocator_.destroy(ptr);
		chunk_pool_.deallocate(ptr);
	}

private:
	enum { DEFAULT_SIZE = 1024 };

	const size_t init_size_;
	mtl::chunk_pool chunk_pool_;
	mtl::allocator<T> allocator_;
};
}; // namespace mtl