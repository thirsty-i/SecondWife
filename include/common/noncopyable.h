#ifndef _H_NONCOPYABLE_H_
#define _H_NONCOPYABLE_H_

	class noncopyable
	{
	protected:
		noncopyable() {}
		~noncopyable() {}

		noncopyable(const noncopyable&) = delete;
		noncopyable& operator=(const noncopyable&) = delete;
	};


#endif