#ifndef _H_STACK_H_
#define _H_STACK_H_

#include <deque>

namespace mtl {
	template <class T, class Sequence = std::deque<T>>
	class stack
	{
	public:
		using value_type = typename Sequence::value_type;
		using pointer = typename Sequence::pointer;
		using reference = typename Sequence::reference;
		using size_type = typename Sequence::size_type;
		using const_reference = typename Sequence::const_reference;

	protected:

	public:
		stack() : container_() {}
		stack(const Sequence& s) : container_(s) {}

		void push(const_reference rNode) { container_.push_back(rNode); }
		void pop() { container_.pop_back(); }
		size_type size() const { container_.size(); }
		bool empty() { return container_.empty(); }

		reference top() { return container_.back(); }
		const_reference top() const { return container_.back(); }
	private:
		Sequence container_;
	};
};
#endif // _H_STACK_H_