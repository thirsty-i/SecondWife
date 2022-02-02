#pragma once

#include "../common/noncopyable.h"

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