#pragma once

#include "../memory/chunk.h"
#include "../memory/memory_resource.h"
#include "../memory/allocator.h"

namespace mtl {
class chunk_pool
{
public:
	using chunk_t = mtl::chunk;

	chunk_pool(size_t block_size, size_t blocks)
		: chunk_pool(block_size, blocks, MAX_RATE) {};

	chunk_pool(size_t block_size, size_t blocks, size_t recycle_rate)
		: chunk_pool(block_size, blocks, recycle_rate, get_new_delete_resource()) {};

	chunk_pool(size_t block_size, size_t blocks, size_t recycle_rate, memory_resource* resource)
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
			<< "is not from this";
	}

	void free_all()
	{
		for (auto& chunk : chunks_)
		{
			resource_->deallocate(chunk.origer_ptr_, chunk.bytes_);
		}
		chunks_.clear();
	}


private:
	void _fill()
	{
		size_t bytes = blocks_ * block_size_;
		static auto deleter = [&](void* ptr) {
			resource_->deallocate(ptr, bytes);
		};

		// emplace_back may throw an exception, use unique_ptr to ensure exception safety
		std::unique_ptr<void, decltype(deleter)> res(resource_->allocate(bytes), deleter);
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

		// no more blocks
		if (chunks_.size() == 1)
			return 0;

		void* p = 0;
		for (chunk_t& chunk : chunks_)
		{
			if (!(p = chunk.pop()))
				continue;

			// This block is not full, move to the back
			if(!chunk.empty())
				std::swap(free_chunk, chunk);
			break;
		}

		return p;
	}

private:
	enum { MAX_RATE = 100 };
	size_t block_size_;
	size_t blocks_;
	size_t recycle_rate_;
	std::vector<chunk_t> chunks_; // 这里要传一个小内存分配器???
	memory_resource* resource_;
};
};