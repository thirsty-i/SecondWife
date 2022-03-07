#pragma once

#include <thread>
#include "mtl/struct/lock_free/queue.h"
#include "common/noncopyable.h"
#include "common/logger/log.h"

class task;
class thread_pool;

using task_func = std::function<void(task&)>;

class task
{
public:
	task(task_func&& work_func)
		: work_func(work_func)
	{
	}

	task(task_func&& work_func, task_func&& finish_func)
		: work_func(work_func)
		, finish_func(finish_func)
	{
	}

	task(task&& t)
		: work_func(std::move(t.work_func))
		, finish_func(std::move(t.finish_func))
	{

	}

	virtual ~task() {}

	task_func work_func;
	task_func finish_func;
};


class worker final
	: private noncopyable
{
public:
	worker(thread_pool* pool)
		: thread_pool_(pool)
	{
		LOG_PROCESS_ERROR(pool);

		start_ = true;
		thread_.reset(new std::thread(&worker::execute, this));
	}

	void add_task(task&& t)
	{
		task_queue_.push(t);
	}

protected:
	void execute()
	{
		task t;
		while (start_)
		{
			if (task_queue_.pop(t))
			{
				t.work_func();
				
				if (t.finish_func)
					thread_pool_->push_task_finish(t);
			}
		}
	}

	virtual ~worker()
	{
		LOG_PROCESS_ERROR(thread_);
		
		if (thread_->joinable())
			thread_->join();
	}

private:
	bool start_;
	mtl::queue<task> task_queue_;
	std::unique_ptr<std::thread> thread_;
	thread_pool* thread_pool_;
};

class thread_pool final
	: private noncopyable
{
public:
	using worker_ptr_t = std::unique_ptr<worker>;
	using thread_num_t = uint16_t;

	thread_pool(thread_num_t thread_num)
		: next_thread_index_(0)
	{
		LOG_PROCESS_ERROR(thread_num);

		for(thread_num_t i = 0; i < thread_num; ++i)
			workers_.push_back(std::make_unique<worker>(this));
	}

	
	void push_task(task&& task, thread_num_t thread_index = 0)
	{
		if (thread_index == 0)
		{
			thread_index = next_thread_index_;
			next_thread_index_ = (next_thread_index_ + 1) % workers_.size();
		}

		LOG_PROCESS_ERROR(thread_index < workers_.size());
		workers_[thread_index]->add_task(task);
	}

	void push_task_finish(task&& task)
	{
		finish_queue_.push(task);
	}

	// FIXME: This API is not thread safe
	uint32_t run_finish()
	{
		uint32_t count = 0;
		task t;

		while (finish_queue_.pop(t))
		{
			t.finish_func();
			++count;
		}
		
		return count;
	}

	// FIXME: This API is not thread safe
	bool run_once_finish()
	{
		task t;

		if (!finish_queue_.pop(t))
			return false;
		
		t.finish_func();
		return true;
	}

private:
	std::vector<worker_ptr_t> workers_;
	mtl::queue<task> finish_queue_;
	thread_num_t next_thread_index_;
};