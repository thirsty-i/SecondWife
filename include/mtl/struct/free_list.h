#pragma once

#include "common/noncopyable.h"
#include "mtl/struct/lock_free/tagged_ptr.h"
#include <type_traits>
#include <memory>
#include "common/logger/log.h"

namespace mtl {
// not thread safe
template <class T, size_t Size>
class free_list : private noncopyable
{
	static_assert(Size, "Size is zero");
public:
	using pointer_t = T*;
	using value_t = T;
	using reference = T&;
	using size_type = size_t;

	union node_t
	{
		union node_t* next;
		T data;
	};

	free_list()
		: head_(0)
		, cnt_(0)
	{
		for (size_type i = 0; i < Size; ++i)
		{
			push(nodes_ + i);
		}
	};

	bool full() noexcept
	{
		return cnt_ == Size;
	}

	bool empty() noexcept
	{
		return !cnt_ && !head_;
	}

	bool push(void* ptr) noexcept
	{
		LOG_PROCESS_ERROR_RET(ptr, false);
		LOG_PROCESS_ERROR_RET(!full(), false);

		node_t* node = static_cast<node_t*>(ptr);
		node->next = head_;
		head_ = node;
		++cnt_;
		return true;
	}

	pointer_t pop()
	{
		if (empty())
			return 0;

		node_t* res = head_;
		head_ = res->next;
		--cnt_;
		return reinterpret_cast<pointer_t>(res);
	}

	bool is_from_this(void* ptr)
	{
		return ptr >= nodes_ && ptr <= nodes_ + Size - 1;
	}

private:
	node_t nodes_[Size];
	node_t* head_;
	size_type cnt_;
};
};
