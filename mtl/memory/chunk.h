#pragma once

#include <type_traits>
#include <memory>
#include "../common/logger/log.h"
#include "../common/noncopyable.h"
#include "memory_resource.h"

namespace mtl {
	// not thread safe
	struct chunk final
	{
		struct link_node
		{
			link_node* next;
		};

		chunk(void* p, size_t bytes, size_t block_size) noexcept
			: origer_ptr_(p)
			, bytes_(bytes)
			, head_(0)
			, unuse_(0)
		{
			LOG_PROCESS_ERROR(block_size >= sizeof(void*));
			LOG_PROCESS_ERROR(origer_ptr_);
			LOG_PROCESS_ERROR(bytes_);
			LOG_PROCESS_ERROR(bytes_ % block_size == 0);

			blocks_ = bytes_ / block_size;

			for (size_t i = 0; i < blocks_; ++i)
			{
				push(reinterpret_cast<char*>(origer_ptr_) + i * block_size);
			}
		}

		chunk& operator=(chunk&& c) noexcept = default;
		chunk(chunk&& c) noexcept = default;

		bool full() noexcept
		{
			return unuse_ == blocks_;
		}

		bool empty() noexcept
		{
			return !head_;
		}

		bool push(void* ptr) noexcept
		{
			LOG_PROCESS_ERROR_RET(ptr, false);
			LOG_PROCESS_ERROR_RET(!full(), false);

			link_node* node = static_cast<link_node*>(ptr);
			node->next = head_;
			head_ = node;
			++unuse_;
			return true;
		}

		void* pop() noexcept
		{
			if (empty())
				return 0;

			link_node* res = head_;
			head_ = res->next;
			--unuse_;
			return res;
		}

		bool is_from(void* ptr) noexcept
		{
			return ptr >= reinterpret_cast<char*>(origer_ptr_)
				&& ptr < reinterpret_cast<char*>(origer_ptr_) + bytes_;
		}

		size_t bytes_;
		void* origer_ptr_;
		size_t blocks_;

		link_node* head_;
		size_t unuse_;
	};
};
