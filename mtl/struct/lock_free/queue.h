#pragma once

#include <type_traits>
#include "readerwriterqueue.h"

namespace mtl {
template <class T>
class queue
{
public:

	void push(T value)
	{
		queue_.enqueue(value);
	}

	bool pop(T& value)
	{
		return queue_.try_dequeue(value);
	}
private:
	moodycamel::ReaderWriterQueue<T> queue_;
};
}