#pragma once

#include <atomic>
#include "mtl/struct/lock_free/tagged_ptr.h"

namespace mtl {
namespace lock_free {
	
	struct stack_node
	{
		stack_node* next;
	};

	class stack
	{
	private:
		using tagged_ptr_t = tagged_ptr<stack_node>;
	public:
		stack() : header_(0) {}

		void push(stack_node* node)
		{
			tagged_ptr_t old_header = header_.load(std::memory_order_acquire);
			tagged_ptr_t new_header = 0;
			do
			{
				node->next = old_header.get_ptr();
				new_header.set(node, old_header.get_tag() + 1);
			} while (header_.compare_exchange_weak(
								  old_header
								, new_header
								, std::memory_order_release
								, std::memory_order_relaxed));
		}

		stack_node* pop()
		{ 
			tagged_ptr_t old_header = header_.load(std::memory_order_acquire);
			tagged_ptr_t new_header = 0;
			stack_node* result = 0;

			do
			{
				result = old_header.get_ptr();

				if (!result)
					return result;

				new_header.set(result->next, old_header.get_tag() + 1)

			} while (header_.compare_exchange_weak(
								  old_header
								, new_header
								, std::memory_order_release
								, std::memory_order_relaxed));

			return result;
		}
	private:
		std::atomic<tagged_ptr_t> header_;
	};
}
};