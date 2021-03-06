#pragma once

#include <memory>
#include <type_traits>
#include "memory_resource.h"
#include "../common/noncopyable.h"
#include "../common/logger/log.h"

namespace mtl {
// adapt STL	
template <class T>
class allocator
{
public:
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using size_type = size_t;
	using void_pointer = void*;
	using difference_type = ptrdiff_t;
	using const_pointer = const T*;
	using const_reference = const T&;

	template <class _Up>
	struct rebind
	{
		typedef allocator<_Up> other;
	};

	static_assert(sizeof(T), "T size is zero!!!");

	allocator(memory_resource* const resource = get_new_delete_resource()) noexcept // strengthened
		: resource_(resource) 
	
	{
		LOG_PROCESS_ERROR(resource_);
	};

	allocator(const allocator&) = default;

	template <class _Uty>
	allocator(const allocator<_Uty>& other) noexcept
		: resource_(other.resource_) {} // initialize with _That's resource;

	allocator& operator=(const allocator&) = delete;

	pointer allocate(size_t count)
	{
		LOG_PROCESS_ERROR_RET(count, 0);

		void* const p = resource_->allocate(count * sizeof(T), alignof(T));
		return static_cast<pointer>(p);
	}

	void deallocate(T* const ptr, size_t count)
	{
		LOG_PROCESS_ERROR(ptr);
		LOG_PROCESS_ERROR(count);

		resource_->deallocate(ptr, sizeof(T) * count, alignof(T));
	}

	template<class ...Args>
	void construct(pointer ptr, Args&&... args)
	{
		::new ((void*)ptr) T(std::forward<Args>(args)...);
	}

	void destroy(pointer ptr)
	{
		ptr->~T();
	}

	size_type max_size() const { return size_type(-1) / sizeof(value_type); }

	pointer address(reference __x) const noexcept { return std::addressof(__x); }
	const_pointer address(const_reference __x) const noexcept { return std::addressof(__x); }

private:
	memory_resource* resource_;
};
} // namsapace mtl