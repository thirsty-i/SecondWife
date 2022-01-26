#pragma once

#include <memory>
#include "common/noncopyable.h"
#include "common/logger/log.h"
#include "mtl/struct/lock_free/free_list.h"

namespace mtl
{
	// This class is thread safe
	template <class T, size_t InitSize>
	class bounded_object_pool final
		: private noncopyable
	{
		static_assert(InitSize >= (1 << 3), "InitSize too small");

	public:
		using chunk_t = mtl::lock_free::free_list<T, InitSize>;
		using value_t = typename chunk_t::value_t;
		using pointer_t = typename chunk_t::pointer_t;

		bounded_object_pool()
			: chunk_(std::make_unique<chunk_t>())
		{

		}

		~bounded_object_pool()
		{
			
		}


		template <class... Args>
		std::shared_ptr<value_t> allocate(Args... args)
		{
			LOG_PROCESS_ERROR_RET(chunk_, 0);

			std::shared_ptr<value_t> res(chunk_->pop(), [this](pointer_t ptr) {
				deallocate(ptr);
			});

			if (!res)
				return res;

			new(res.get()) T(std::forward<Args>(args)...);
			LOG(DEBUG) << "allocate, ptr:" << res.get();

			return res;
		}

		void deallocate(pointer_t ptr)
		{
			LOG_PROCESS_ERROR(ptr);
			LOG_PROCESS_ERROR(chunk_);
			LOG_PROCESS_ERROR(chunk_->is_from_this(ptr));

			(ptr)->value_t::~value_t();
			chunk_->push(ptr);

			LOG(DEBUG) << "deallocate, ptr:" << ptr;
		}

	private:
		std::unique_ptr<chunk_t> chunk_;
	};
};