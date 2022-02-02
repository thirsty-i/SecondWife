#pragma once

class null_lock
{
public:
	null_lock() = default;
				   
	bool try_lock{ return true; }
	void lock() { /*do nothing*/ }
	void unlock() { /*do nothing*/}
};