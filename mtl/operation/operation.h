#pragma once
#include <functional>

class operation
{
public:
	typedef std::function<void(void)> functor_t;

	operation() {}

	void call_work() 
	{
		if (work_handler_)
			work_handler_();
	}

	void call_complete()
	{
		if (complete_handler_)
			complete_handler_();
	}

	void set_work_callback(functor_t f) { work_handler_ = f; }
	void set_complete_callback(functor_t f) { complete_handler_ = f; }

public:
	operation* next_;
private:
	functor_t work_handler_;
	functor_t complete_handler_;
};