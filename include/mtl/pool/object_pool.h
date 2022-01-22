#ifndef _H_OBJECT_POOL_H_
#define	_H_OBJECT_POOL_H_

#include "mtl/struct/lock_free/stack.h"
#include <utility>
#include "mtl/thread/null_lock.hpp"
#include "common/noncopyable.h"
#include <mutex>

namespace mtl
{
	template <class T, bool Lock, class Alloc = std::allocator<T>>
	class object_pool final
		: private noncopyable
	{
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using size_type = size_t;
		using allocator_type = typename Alloc::template rebind<value_type>::other;
		using lock_type = typename std::conditional<Lock, std::mutex, null_lock>::type;

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

			//else
			//{
			//	node = free_stack_.top();
			//	free_stack_.pop();
			//}

			if (node)
				allocator_.construct(node, std::forward<Args>(args)...);

			return node;
		}

		void release(pointer node)
		{
			//std::lock_guard<lock_type> lock_guard(lock_);
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
		lock_type lock_;
	};
};
#endif // _H_OBJECT_POOL_H_