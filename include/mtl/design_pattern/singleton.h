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
		static const T& GetInstance()
		{
			if (instance_ == nullptr)
				instance_ = new T;

			return *instance_;		

		singleton(T&&) = delete;

	protected:
		singleton(void) = default;
		virtual ~singleton()
		{
			delete instance_;
		};

		static T* instance_;
	};

	template <class T>
	T* singleton<T>::instance_ = nullptr;
};
#endif // _H_SINGLETON_H_