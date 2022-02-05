#pragma once

#include <atomic>
#include "tagged_ptr.h"
#include "../common/noncopyable.h"
#include "../common/logger/log.h"

namespace mtl {
namespace lock_free {
// thread safe
class chunk 
	: private noncopyable
{
public:
	chunk(size_t bytes, size_t block_size)
		: origer_ptr_(static_cast<char*>(::operator new (bytes)))
		, bytes_(bytes)
		, head_(tagged_ptr_t(0))
	{
		LOG_PROCESS_ERROR(bytes > block_size);
		LOG_PROCESS_ERROR(block_size >= sizeof(void*));
		LOG_PROCESS_ERROR(origer_ptr_);
		LOG_PROCESS_ERROR(bytes_);
		LOG_PROCESS_ERROR(bytes_ % block_size == 0);

		blocks_ = bytes_ / block_size;

		for (size_t i = 0; i < blocks_; ++i)
		{
			push(origer_ptr_ + i * block_size);
		}
	}
	
	~chunk()
	{
		::operator delete(origer_ptr_);
	}

	chunk& operator=(chunk&& c) noexcept = default;
	chunk(chunk && c) noexcept = default;

	void push(void* ptr) noexcept
	{
		LOG_PROCESS_ERROR(ptr);

		tagged_ptr_t old_head = head_.load(std::memory_order_acquire);
		tagged_ptr_t new_head(0);
		link_node* node = static_cast<link_node*>(ptr);

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

	void* pop()
	{
		tagged_ptr_t old_head = head_.load(std::memory_order_acquire);
		tagged_ptr_t new_head(0);
		link_node* result = 0;

		do
		{
			result = old_head.get_ptr();

			if (!result)
				return result;

			new_head.set(result->next, old_head.get_tag() + 1);
		} while (head_.compare_exchange_weak(
											old_head
											, new_head
											, std::memory_order_release
											, std::memory_order_relaxed));

		return result;
	}

	bool is_from(void* ptr) noexcept
	{
		return ptr >= reinterpret_cast<char*>(origer_ptr_)
			&& ptr < reinterpret_cast<char*>(origer_ptr_) + bytes_;
	}

private:
	struct link_node
	{
		link_node* next;
	};

	using tagged_ptr_t = tagged_ptr<link_node>;

	size_t bytes_;
	char* origer_ptr_;
	size_t blocks_;

	std::atomic<tagged_ptr_t> head_;
	size_t unuse_;
};
} // namespace lock_free
} // namespace mtl
