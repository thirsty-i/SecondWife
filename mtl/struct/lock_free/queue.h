#pragma once

#include <type_traits>
#include <queue>
#include "thread/spin_lock.hpp"
#include <mutex>

// Pretend this is a lock-free queue
namespace mtl {
template <class T>
class queue
{
public:
	static_assert(!std::is_pointer<T>::value, "Template parameter cannot be a pointer");
	using pointer = T*;

	void push(pointer ptr)
	{
		std::lock_guard<spin_lock> guard(lock_);
		queue_.push(ptr);
	}

	pointer pop()
	{
		std::lock_guard<spin_lock> guard(lock_);
		
		if (queue_.empty())
			return 0;

		pointer result = queue_.front();
		queue_.pop();
		return result;
	}
private:
	spin_lock lock_;
	std::queue<pointer> queue_;
};
}