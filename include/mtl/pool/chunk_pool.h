#pragma once

#include "mtl/memory/chunk.h"
#include "mtl/memory/memory_resource.h"
#include "mtl/memory/allocator.h"

namespace mtl {
class chunk_pool
{
public:
	using chunk_t = mtl::chunk;

	chunk_pool(size_t block_size, size_t blocks, size_t recycle_rate = 100, memory_resource* resource = get_new_delete_resource())
		: block_size_(block_size)
		, blocks_(blocks)
		, recycle_rate_(recycle_rate)
		, resource_(resource)
	{
		LOG_PROCESS_ERROR(block_size_);
		LOG_PROCESS_ERROR(blocks_);
		LOG_PROCESS_ERROR(recycle_rate_);
		LOG_PROCESS_ERROR(resource_);

	}

	void* allocate()
	{
		if (void* p = _try_allocate())
			return p;

		_fill();
		return chunks_.back().pop();
	}

	void deallocate(void* ptr)
	{
		LOG_PROCESS_ERROR(!chunks_.empty());
		LOG_PROCESS_ERROR(ptr);

		// The last piece of no recovery
		if (chunks_.back().is_from(ptr))
		{
			chunks_.back().push(ptr);
			return;
		}

		for (chunk_t& chunk : chunks_)
		{
			if (chunk.is_from(ptr))
			{
				chunk.push(ptr);
				_try_recycle(chunk);
				return;
			}
		}
		LOG(ERROR) << "ptr:" << ptr
			"is not from this";
	}


private:
	void _fill()
	{
		mtl::allocator<char> allocator(memory_resource_);
		std::unique_ptr<char> res(allocator.allocate(blocks_), [&](char* ptr) {
			allocator.deallocate(ptr, blocks_);
		});

		chunks_.emplace_back(res.get(), blocks_ * block_size_, block_size_);

		res.release();
	}

	void _try_recycle(chunk_t& chunk)
	{
		if (!chunk.full())
			return;

		if (_can_recycle())
		{
			resource_->deallocate(chunk.origer_ptr_, chunk.bytes_);
			// chunk it's not the back
			std::swap(chunk, chunks_.back());
			chunks_.pop_back();
		}
		return;
	}

	bool _can_recycle()
	{
		size_t free_cnt = 0;

		for (chunk_t& chunk : chunks_)
		{
			if (chunk.full())
				++free_cnt;
		}

		enum { MAX_RATE = 100 };
		return (chunks_.size() - free_cnt) * recycle_rate_ / MAX_RATE;
	}

	void* _try_allocate()
		noexcept(std::is_nothrow_move_assignable<chunk_t>::value
			&& std::is_nothrow_move_assignable<chunk_t>::value)
	{
		if (chunks_.empty())
			return 0;

		chunk_t& free_chunk = chunks_.back();
		if (void* p = free_chunk.pop())
			return p;

		if (chunks_.size() == 1)
			return 0;

		void* p = 0;
		for (chunk_t& chunk : chunks_)
		{
			if (p = chunk.pop())
			{
				std::swap(free_chunk, chunk);
				break;
			}
		}

		return p;
	}

	size_t block_size_;
	size_t blocks_;
	size_t recycle_rate_;
	std::vector<chunk_t> chunks_; // 这里要传一个小内存分配器
};
};