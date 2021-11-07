#ifndef _H_LOCK_GUARD_H_
#define _H_LOCK_GUARD_H_

template <class _LockType>
class CLockGuard
{
public:
	typedef _LockType lock_type;
	typedef lock_type& reference;

	explicit CLockGuard(reference lock) : m_Lock(lock)
	{
		m_Lock.Lock();
	}

	~CLockGuard()
	{
		m_Lock.UnLock();
	}

	CLockGuard& operator=(CLockGuard&) = delete;
	CLockGuard(const CLockGuard&) = delete;

private:
	reference m_Lock;
};

#endif // _H_LOCK_GUARD_H_