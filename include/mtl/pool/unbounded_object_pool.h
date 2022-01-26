#pragma once

#include <utility>
#include <list>
#include "common/noncopyable.h"
#include "mtl/struct/free_list.h"
#include "mtl/pool/chunk_pool.h"

#include "common/logger/log.h"

namespace mtl
{
	// This class is not thread safe
	template <class T, size_t InitSize>
	class unbounded_object_pool final
		: private noncopyable
	{
	public:
		using chunk_t = free_list<T, InitSize>;

		using pointer_t = typename chunk_t::pointer_t;
		using reference = typename chunk_t::reference;
		using size_type = typename chunk_t::size_type;

		using chunk_pool_t = mtl::chunk_pool<chunk_t>;
		using chunk_smart_ptr_t = typename chunk_pool_t::shared_ptr_t;

		static_assert(InitSize >= (1 << 3), "InitSize too small");

		unbounded_object_pool() 
			: last_chunk_(0)
			, chunk_pool_()
		{
			last_chunk_ = chunk_pool_.allocate();
		};

		~unbounded_object_pool() {
			LOG_PROCESS_ERROR(last_chunk_->full());
			LOG_PROCESS_ERROR(chunks_.size() == 0);
		};

		template <class... Args>
		std::shared_ptr<T> allocate(Args&&... args)
		{
			LOG_PROCESS_ERROR_RET(last_chunk_, 0);

			if (last_chunk_->empty())
				_allocate_chunk();

			std::shared_ptr<T> res(last_chunk_->pop(), [this](pointer_t ptr) {
				deallocate(ptr);
			});

			new(res.get()) T(std::forward<Args>(args)...);

			LOG(DEBUG) << "allocate, ptr:" << res.get();
			return res;
		}

		void deallocate(pointer_t ptr)
		{
			LOG_PROCESS_ERROR(ptr);
			LOG_PROCESS_ERROR(last_chunk_);

			(ptr)->T::~T();

			LOG(DEBUG) << "deallocate, ptr:" << ptr;

			if (last_chunk_->is_from_this(ptr))
			{
				last_chunk_->push(ptr);
				return;
			}

			for (chunk_smart_ptr_t& chunk : chunks_)
			{
				if (chunk->is_from_this(ptr))
				{
					chunk->push(ptr);
					if (chunk->full())
						_deallocate_chunk(chunk);
					return;
				}
			}

			LOG(ERROR) << "ptr:" << ptr
				<< " not match chunk";
		}

	private:
		void _allocate_chunk()
		{
			chunks_.push_back(last_chunk_);
			last_chunk_ = chunk_pool_.allocate();
		}

		void _deallocate_chunk(chunk_smart_ptr_t& chunk)
		{
			chunk_pool_.deallocate(std::move(chunk));
			chunks_.remove(chunk);
		}

		chunk_smart_ptr_t last_chunk_;
		std::list<chunk_smart_ptr_t /*small memory allocator???*/> chunks_;
		chunk_pool_t chunk_pool_;
	};
};