# pragma once

#include "noncopyable.h"

namespace mtl {

template <typename Operation>
class op_queue;

class op_queue_access
{
public:
	template <typename Operation>
	static Operation* next(Operation* o)
	{
		return static_cast<Operation*>(o->next_);
	}

	template <typename Operation1, typename Operation2>
	static void next(Operation1*& o1, Operation2* o2)
	{
		o1->next_ = o2;
	}

	template <typename Operation>
	static Operation*& front(op_queue<Operation>& q)
	{
		return q.front_;
	}

	template <typename Operation>
	static Operation*& back(op_queue<Operation>& q)
	{
		return q.back_;
	}
};

template <typename Operation>
class op_queue
	: private noncopyable
{
public:
	op_queue()
		: front_(0),
		back_(0)
	{
	}

	~op_queue()
	{
		while (Operation* op = front_)
		{
			pop();
		}
	}

	Operation* front()
	{
		return front_;
	}

	void pop()
	{
		if (front_)
		{
			Operation* tmp = front_;
			front_ = op_queue_access::next(front_);
			if (front_ == 0)
				back_ = 0;
			op_queue_access::next(tmp, static_cast<Operation*>(0));
		}
	}

	void push(Operation* h)
	{
		op_queue_access::next(h, static_cast<Operation*>(0));
		if (back_)
		{
			op_queue_access::next(back_, h);
			back_ = h;
		}
		else
		{
			front_ = back_ = h;
		}
	}

	template <typename OtherOperation>
	void push(op_queue<OtherOperation>& q)
	{
		if (Operation* other_front = op_queue_access::front(q))
		{
			if (back_)
				op_queue_access::next(back_, other_front);
			else
				front_ = other_front;
			back_ = op_queue_access::back(q);
			op_queue_access::front(q) = 0;
			op_queue_access::back(q) = 0;
		}
	}

	bool empty() const
	{
		return front_ == 0;
	}

	bool is_enqueued(Operation* o) const
	{
		return op_queue_access::next(o) != 0 || back_ == o;
	}

private:
	friend class op_queue_access;

	Operation* front_;
	Operation* back_;
};
} // namespace mtl