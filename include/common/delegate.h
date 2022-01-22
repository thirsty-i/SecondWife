#ifndef _HPP_DELEGATE_HPP_
#define _HPP_DELEGATE_HPP_

#include <queue>
#include <functional>
#include <utility>

namespace mtl {

	template <class _Ref, class... _Types>
	class delegate final
	{
	public:
		typedef std::function<void(_Types...)> functor_type;
		
		~delegate()
		{
			functors_.clear();
		}

		void register_to(functor_type&& func)
		{
			functors_.push_back(std::forward<functor_type>(func));
		}

		void invoke(_Types&&... args)
		{
			for (const functor_type& func : functors_)
			{
				(func)(std::forward<_Types>(args)...);
			}
		}
	private:
		std::queue<functor_type> functors_;
	};
};
#endif // _HPP_DELEGATE_HPP_