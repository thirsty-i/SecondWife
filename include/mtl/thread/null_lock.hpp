#ifndef _HPP_NULL_LOCK_HPP_
#define _HPP_NULL_LOCK_HPP_

class CNullLock
{
public:
	CNullLock() = default;

	void Lock() { /*do nothing*/ }
	void UnLock() { /*do nothing*/}
};


#endif // _HPP_NULL_LOCK_HPP_