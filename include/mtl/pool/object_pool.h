#pragma  once

#include "memory_resource.h"
#include "chunk_pool.h"
#include "allocator.h"
#include "chunk.h"
#include <type_traits>

template<class T>
class object_pool
{
public:
	object_pool(size_t size)
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
		std::shared_ptr<T> res(chunk_pool_.allocate(), [this](T* ptr) {
			release(ptr);
		});

		LOG_PROCESS_ERROR(res.get());

		allocator_.construct(res.get(), std::forward<Args>(args)...);

		return res;
	}

	void release(void* ptr)
	{
		allocator_.destroy(ptr);
		chunk_pool_.deallocate(ptr);
	}
private:
	mtl::chunk_pool chunk_pool_;
	mtl::allocator<T> allocator_;
};