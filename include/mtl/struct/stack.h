#ifndef _H_STACK_H_
#define _H_STACK_H_

#include <deque>

namespace mtl {
	template <class T, class Sequence = std::deque<T>>
	class CStack
	{
	public:
		using value_type = typename Sequence::value_type;
		using pointer = typename Sequence::pointer;
		using reference = typename Sequence::reference;
		using size_type = typename Sequence::size_type;
		using const_reference = typename Sequence::const_reference;

	protected:

	public:
		CStack() : m_Container() {}
		CStack(const Sequence& s) : m_Container(s) {}

		void Push(const_reference rNode) { m_Container.push_back(rNode); }
		void Pop() { m_Container.pop_back(); }
		size_type Size() const { m_Container.size(); }
		bool Empty() { return m_Container.empty(); }

		reference Top() { return m_Container.back(); }
		const_reference Top() const { return m_Container.back(); }
	private:
		Sequence m_Container;
	};
};
#endif // _H_STACK_H_