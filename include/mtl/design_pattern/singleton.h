#ifndef _H_SINGLETON_H_
#define _H_SINGLETON_H_

#include "common/noncopyable.h"

namespace mtl
{
	template <class T>
	class singleton
		: private noncopyable
	{
	public:
		static T& instance()
		{
			return instance_;
		}

		singleton(T&&) = delete;
	protected:
		singleton(void) = default;
		static T instance_;
	};

	template <class T>
	T singleton<T>::instance_;
};
#endif // _H_SINGLETON_H_