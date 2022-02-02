#pragma once

#include "../common/noncopyable.h"
#include <atomic>

class spin_lock
	: private noncopyable
{
public:
	bool try_lock()	noexcept
	{
		return !lock_.load(std::memory_order_relaxed)
			&& !lock_.exchange(true, std::memory_order_acquire);
	}

	void lock()	noexcept
	{
		if (!lock_.exchange(true, std::memory_order_acquire)) {
			return;
		}

		while (lock_.load(std::memory_order_relaxed)) {
			/*busy waiting*/
		}
	}

	void unlock() noexcept
	{
		lock_.store(false, std::memory_order_release);
	}

private:
	std::atomic<bool> lock_ = { false };
};