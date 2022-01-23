#pragma once

#include <atomic>
#include "mtl/struct/lock_free/tagged_ptr.h"
#include "common/node.h"

namespace mtl {
namespace lock_free {
	
	class stack
	{
	public:
		using node_t = forward_node;
		using tagged_ptr_t = tagged_ptr<node_t>;

		stack() : header_(tagged_ptr_t(0)) {}

		void push(node_t* node)
		{
			tagged_ptr_t old_header = header_.load(std::memory_order_acquire);
			tagged_ptr_t new_header(0);
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

		node_t* pop()
		{ 
			tagged_ptr_t old_header = header_.load(std::memory_order_acquire);
			tagged_ptr_t new_header(0);
			node_t* result = 0;

			do
			{
				result = old_header.get_ptr();

				if (!result)
					return result;

				new_header.set(result->next, old_header.get_tag() + 1);
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