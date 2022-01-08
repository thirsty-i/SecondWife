#ifndef _H_MAP_H_
#define _H_MAP_H_

#include "mtl/struct/tree/rb_tree.h"
#include "mtl/memory/allocator.h"

namespace mtl
{
	template <
		class _Key,
		class _Value,
		class _Compare = std::less<_Key>,
		class _Alloc = allocator<std::pair<const _Key, _Value>>
	>
		class map
	{
	public:
		using pair_type = std::pair<const _Key, _Value>;
		using allocator_type = _Alloc;

		template <class _compare_type>
		struct pair_compare
		{
			_compare_type m_Compare;

			bool operator()(const pair_type& rFirst, const pair_type& rSecond) const
			{
				return m_Compare(rFirst.first, rSecond.first);
			}

			bool operator()(const _Key& rFirst, const pair_type& rSecond) const
			{
				return m_Compare(rFirst, rSecond.first);
			}

			bool operator()(const pair_type& rFirst, const _Key& rSecond) const
			{
				return m_Compare(rFirst.first, rSecond);
			}
		};

	private:
		using _impl_type = typename mtl::_Rb_tree<_Key, pair_type, pair_compare<_Compare>, allocator_type>;
	public:	
		using iterator       = typename _impl_type::iterator;
		using const_iterator = typename _impl_type::const_iterator;
		using size_type      = typename _impl_type::size_type;

		map() : rb_tree_() {}
	public:
		iterator insert(const pair_type& pair) { return rb_tree_.insert_unique(pair); }
		iterator find(const _Key& key) { return rb_tree_.find(key); }
		iterator find(_Key&& key) { return rb_tree_.find(key); }
		void erase(iterator iter) { return rb_tree_.erase(iter); }
		size_type size() { return rb_tree_.size(); }

		iterator begin() { return rb_tree_.begin(); }
		iterator end() { return rb_tree_.end(); }

		//const_iterator find(const _Key& key) const { return rb_tree_.Find(key); }
		//const_iterator begin() const { return rb_tree_.Begin(); }
		//const_iterator end() const { return rb_tree_.End(); }

		void print_tree() { rb_tree_.print_tree();  }
	private:
		_impl_type rb_tree_;
	};
}

#endif //_H_MAP_H_