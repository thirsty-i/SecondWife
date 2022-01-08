#ifndef _HPP_NULL_LOCK_HPP_
#define _HPP_NULL_LOCK_HPP_

class null_lock
{
public:
	null_lock() = default;
				   
	bool try_lock{ return true; }
	void lock() { /*do nothing*/ }
	void unlock() { /*do nothing*/}
};


#endif // _HPP_NULL_LOCK_HPP_