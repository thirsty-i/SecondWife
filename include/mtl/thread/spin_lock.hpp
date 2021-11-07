#ifndef _HPP_SPIN_LOCK_HPP_
#define _HPP_SPIN_LOCK_HPP_

#include <atomic>

class CSpinLock
{
private:
	typedef enum { Locked, Unlocked } LockState;
	std::atomic<LockState> m_State;

public:
	CSpinLock() : m_State(Unlocked) {};

	void Lock()
	{
		while (m_State.exchange(Locked, std::memory_order_acquire) == Locked) {
			/* busy-wait */
		}
	}
	void UnLock()
	{
		m_State.store(Unlocked, std::memory_order_release);
	}
};

#endif // _HPP_SPIN_LOCK_HPP_