#pragma once 

class memory_resource
{
public:
	using resource_smart_ptr_t = std::shared_ptr<memory_resource>;

protected:
	virtual ~memory_resource() noexcept = default;

	virtual void* allocate(size_t bytes, size_t alignment) = 0;
	virtual void  deallocate(void* ptr, size_t bytes, size_t alignment) = 0;
};