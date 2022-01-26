#pragma once

#include "common/noncopyable.h"
#include <list>
#include <memory>
#include <functional>
#include <type_traits>
#include "common/logger/log.h"

namespace mtl {
template <class T>
class chunk_pool final
	: private noncopyable
{
public:
	using chunk_t = T;
	using size_type = size_t;
	using shared_ptr_t = std::shared_ptr<chunk_t>;

	typedef size_type(*func_type)(size_type, size_type);

	chunk_pool(func_type func = &_get_recycle_number) 
		: func_(func)
		, chunk_cnt_(0) {};

	void deallocate(shared_ptr_t&& chunk)
	{
		free_list.push_back(std::move(chunk));
		_recyclable();
	}

	template <class... Args>
	shared_ptr_t allocate(Args&&... args)
	{
		shared_ptr_t res;
		if (free_list.empty())
		{
			res = std::make_shared<chunk_t>(std::forward<Args>(args)...);
			++chunk_cnt_;
		}
		else
		{
			res.swap(free_list.front());
			free_list.pop_front();
		}

		return res;
	}

private:
	static size_t _get_recycle_number(size_type free_cnt, size_type total_cnt)
	{
		enum { RECYCLE = 2 };
		return free_cnt - (total_cnt - free_cnt + RECYCLE) / RECYCLE;
	}

	void _recyclable()
	{
		size_t number = func_(free_list.size(), chunk_cnt_);
		
		if (!number)
			return;

		LOG_PROCESS_ERROR(number <= free_list.size());

		free_list.remove_if([&number, this](shared_ptr_t&) { 
			if (number--)
			{
				--chunk_cnt_;
				return true;
			}
			return false;
		});
	}

private:
	func_type func_;
	std::list<shared_ptr_t> free_list; 
	size_type chunk_cnt_;
};
}