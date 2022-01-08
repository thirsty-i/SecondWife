#ifndef _H_OBJECT_POOL_H_
#define	_H_OBJECT_POOL_H_

#include "mtl/memory/allocator.h"
#include "stack.h"
#include <deque>
#include <utility>
#include "mtl/thread/null_lock.hpp"
#include "common/noncopyable.h"
#include <list>
#include <mutex>

namespace mtl
{
	template <class T, bool Lock, class Alloc = allocator<T>>
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
			Clear();
			// TODO: check allocate.count == deallocate.cout?
		};

	protected:
		pointer _refill()
		{
			size_type refillSize = _get_refill_size();
			pointer chunk = allocator_.allocate(refillSize);
			size_ += refillSize;

			while (--refillSize)
				free_stack_.push(chunk + refillSize);

			chunks_.push_back(chunk);
			return chunk; // return object memory
		}

		size_type _get_refill_size() { return size_; }

	public:
		void Clear()
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
			std::lock_guard<lock_type> lock_guard(lock_);
			pointer pNode = 0;
			if (free_stack_.empty())
				pNode = _refill();
			else
			{
				pNode = free_stack_.top();
				free_stack_.pop();
			}

			if (pNode)
				allocator_.construct(pNode, std::forward<Args>(args)...);

			return pNode;
		}

		void release(pointer pNode)
		{
			std::lock_guard<lock_type> lock_guard(lock_);
			allocator_.destroy(pNode);
			free_stack_.push(pNode);
		}
	protected:
		std::list<pointer> chunks_;
		stack<pointer>  free_stack_;
		allocator_type allocator_;	  // ¸Ä³ÉÖ¸Õë?
		size_type size_;
		lock_type lock_;
	};
};
#endif // _H_OBJECT_POOL_H_