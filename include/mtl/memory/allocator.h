#pragma once

template <class T>
class allocator
{
	using value_t = T;
	using pointer_t = T*;

	pointer_t allocate(size_t count)
	{
		void* const p = do_allocate(count * sizeof(value_t), alignof(value_t));
		return static_cast<pointer_t>(p);
	}

	void deallocate(value_t* const ptr, size_t count)
	{
		do_deallocate(ptr, sizeof(value_t) * count, alignof(value_t));
	}

private:
	virtual void* do_allocate(size_t bytes, size_t alignment) = 0;
	virtual void do_deallocate(void* ptr, size_t bytes, size_t alignment) = 0;
};