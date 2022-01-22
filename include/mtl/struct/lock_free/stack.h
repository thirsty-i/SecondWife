#ifndef _H_STACK_H_
#define _H_STACK_H_

#include <atomic>

namespace mtl {
namespace lock_free {
	
	struct stack_node
	{
		stack_node* next;
	};

	class stack
	{
	private:
		union stack_head
		{
			stack_node* head;
			struct
			{
				unsigned pointer : 48;
				unsigned version : 16;
			};
		};
	public:
		stack() : header_(0) {}

		void push(stack_node* node)
		{
			stack_head old_header = header_.load(std::memory_order_acquire);
			stack_head new_header = 0;
			do
			{
				node->next = old_header;
				new_header.pointer = static_cast<stack_head>(node);
				new_header.version = old_header.version + 1;
			} while (header_.compare_exchange_weak(
								  old_header
								, new_header
								, std::memory_order_release
								, std::memory_order_relaxed));
		}

		stack_node* pop()
		{ 
			stack_head old_header = header_.load(std::memory_order_acquire);
			stack_head new_header = 0;
			stack_node* result = 0;

			do
			{
				result = old_header.pointer;
				if (!result)
					return result;

				new_header = result->next;
				new_header->version = old_header.version + 1;
			} while (header_.compare_exchange_weak(
								  old_header
								, new_header
								, std::memory_order_release
								, std::memory_order_relaxed));

			return result;
		}
	private:
		std::atomic<stack_head> header_;
	};
}
};
#endif // _H_STACK_H_