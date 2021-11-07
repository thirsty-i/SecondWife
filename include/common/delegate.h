#ifndef _HPP_DELEGATE_HPP_
#define _HPP_DELEGATE_HPP_

#include <list>
#include <functional>
#include <utility>

namespace mtl {
	template <class T>
	class CDelegate	final
	{
	public:
		~CDelegate()
		{
			m_lst.clear();
		}

		void operator+(std::function<T> func)
		{
			m_lst.push_back(func);
		}

		CDelegate& operator+=(std::function<T> func)
		{
			m_lst.push_back(func);
			return *this;
		}

		template <class... _Types >
		void Invoke(_Types&&... args)
		{
			for (auto func : m_lst)
			{
				(func)(std::forward<_Types>(args)...);
			}
		}
	private:
		std::list<std::function<T>> m_lst;
	};
};
#endif // _HPP_DELEGATE_HPP_