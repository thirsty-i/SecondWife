#pragma once

#include "construct.h"
#include <memory>  // address

namespace mtl {
	template <class T>
	class _new_delete_allocator
	{
	public:
		typedef T  value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef size_t size_type;
		typedef void* void_pointer;
		typedef ptrdiff_t  difference_type;
		typedef const T* const_pointer;
		typedef const T& const_reference;

		template <class _Up>
		struct rebind
		{
			typedef _new_delete_allocator<_Up> other;
		};

		_new_delete_allocator() noexcept {}
		_new_delete_allocator(const _new_delete_allocator&) noexcept {}

		template <class _Up>
		_new_delete_allocator(const _new_delete_allocator<_Up>&) noexcept {}
		~_new_delete_allocator() noexcept {}

		pointer allocate(size_type n)
		{
			if (n == 0)
				return 0;
			// 这里可以做一些字节对齐
			return static_cast<pointer>(::operator new(sizeof(T) * n));
		}

		void deallocate(pointer p)
		{
			::operator delete(p);
		}

		template <class... Args>
		void construct(pointer __p, Args&&... args)
		{
			::construct(__p, std::forward<Args>(args)...);
		}

		void destroy(pointer __p)
		{
			::destroy(__p);
		}

		size_type max_size() const { return size_type(-1) / sizeof(value_type); }

		pointer address(reference __x) const noexcept { return std::addressof(__x); }
		const_pointer address(const_reference __x) const noexcept { return std::addressof(__x); }
	};

	template <class _Type>
	using allocator = mtl::_new_delete_allocator<_Type>;
};