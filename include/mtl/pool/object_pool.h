#pragma once

#include "mtl/struct/lock_free/stack.h"
#include <utility>
#include "mtl/thread/null_lock.hpp"
#include "common/noncopyable.h"
#include <mutex>

namespace mtl
{
	template <class T, bool Bounded, class Alloc = std::allocator<T>>
	class object_pool final
		: private noncopyable
	{
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using size_type = size_t;
		using allocator_type = typename Alloc::template rebind<value_type>::other;

		explicit object_pool(size_type size) : size_(size) {};
		~object_pool() {
			clear();
			// TODO: check allocate.count == deallocate.cout?
		};
	public:
		void clear()
		{
			while (!chunks_.empty())
			{
				allocator_.deallocate(chunks_.front());
				chunks_.pop_front();
			}
		}

		template <class... Args>
		pointer create(Args&&... args)
		{
			pointer node = static_cast<pointer>(free_stack_.pop());

			if (!node)
				_refill();

			if (node)
				allocator_.construct(node, std::forward<Args>(args)...);

			return node;
		}

		void release(pointer node)
		{
			allocator_.destroy(node);
			free_stack_.push(static_cast<stack_node*>(node));
		}

	private:
		pointer _refill()
		{
			size_type refill_size = _get_refill_size();
			pointer chunk = allocator_.allocate(refill_size);
			size_ += refill_size;

			while (--refill_size)
				free_stack_.push(chunk + refill_size);

			chunks_.push_back(chunk);
			return chunk; // return object memory
		}

		size_type _get_refill_size() { return size_; }
	private:
		std::list<pointer> chunks_;
		lock_free::stack<pointer>  free_stack_;
		allocator_type allocator_;	  // ¸Ä³ÉÖ¸Õë?
		size_type size_;
	};
};