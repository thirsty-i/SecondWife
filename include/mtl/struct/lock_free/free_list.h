#pragma once

#include <atomic>
#include "mtl/struct/lock_free/tagged_ptr.h"

namespace mtl {
namespace lock_free {
	template <class T, size_t Size>
	class free_list
	{
		static_assert(Size, "Size is zero");
	public:
		union node_t
		{
			union node_t* next;
			T data;
		};

		using tagged_ptr_t = tagged_ptr<node_t>;
		using value_t = T;
		using size_type = size_t;
		using pointer_t = T*;

		free_list() : head_(tagged_ptr_t(0)) 
		{
			for (size_type i = 0; i < Size; ++i)
			{
				push(nodes_ + i);
			}
		}

		void push(void* ptr)
		{
			LOG_PROCESS_ERROR(ptr);

			tagged_ptr_t old_head = head_.load(std::memory_order_acquire);
			tagged_ptr_t new_head(0);
			node_t* node = static_cast<node_t*>(ptr);

			do
			{
				node->next = old_head.get_ptr();
				new_head.set(node, old_head.get_tag() + 1);
			} while (head_.compare_exchange_weak(
								  old_head
								, new_head
								, std::memory_order_release
								, std::memory_order_relaxed));
		}

		pointer_t pop()
		{ 
			tagged_ptr_t old_head = head_.load(std::memory_order_acquire);
			tagged_ptr_t new_head(0);
			node_t* result = 0;

			do
			{
				result = old_head.get_ptr();

				if (!result)
					return reinterpret_cast<pointer_t>(result);

				new_head.set(result->next, old_head.get_tag() + 1);
			} while (head_.compare_exchange_weak(
								  old_head
								, new_head
								, std::memory_order_release
								, std::memory_order_relaxed));

			return reinterpret_cast<pointer_t>(result);
		}

		bool is_from_this(void* ptr)
		{
			return ptr >= nodes_ && ptr <= nodes_ + Size - 1;
		}

	private:
		std::atomic<tagged_ptr_t> head_;
		node_t nodes_[Size];
	};
}
};