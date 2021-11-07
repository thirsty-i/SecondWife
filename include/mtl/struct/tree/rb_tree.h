#ifndef _H_RB_TREE_H_
#define _H_RB_TREE_H_

#include "memory.h"
#include "bs_tree.h"
#include "print_tree.h"

namespace mtl
{
	enum class _Rb_tree_color { rbtcRed = false, rbtcBlack = true };

	struct _Rb_tree_node_base
	{
		typedef _Rb_tree_node_base* node_base_ptr;
		typedef const _Rb_tree_node_base* const_node_base_ptr;

		_Rb_tree_node_base()
		{
			pParent = pLeft = pRight = 0;
			bColor = _Rb_tree_color::rbtcRed;
		}

		_Rb_tree_color& color(void)
		{
			return bColor;
		}

		void clone(node_base_ptr pNode)
		{
			pNode->pParent = pParent;
			pNode->pLeft = pLeft;
			pNode->pRight = pRight;
			pNode->bColor = bColor;
		}

		node_base_ptr pParent;
		node_base_ptr pLeft;
		node_base_ptr pRight;
		_Rb_tree_color bColor;
	};

	template <typename _Value>
	struct _Rb_tree_node : public _Rb_tree_node_base
	{
		typedef _Value value_type;
		typedef _Rb_tree_node<value_type>* node_ptr;
		
		_Rb_tree_node(const value_type& value) : _Rb_tree_node_base(), value(value) {}
		_Rb_tree_node(value_type& value) : _Rb_tree_node_base(), value(value) {}
		_Rb_tree_node(value_type&& value) : _Rb_tree_node_base(), value(value) {}

		node_ptr& left()
		{
			return (node_ptr&)pLeft;
		}

		node_ptr& right()
		{
			return (node_ptr&)pRight;
		}

		node_ptr& parent()
		{
			return (node_ptr&)pParent;
		}

		value_type value;
	};

	template <class _Value, class _Ptr, class _Ref>
	struct _Rb_tree_iterator
	{
		typedef _Value    value_type;
		typedef _Value&   reference;
		typedef _Value*   pointer;
		//typedef bidirectional_iterator_tag iterator_category;
		typedef _Rb_tree_iterator<_Value, _Ptr, _Ref> iterator;
		typedef _Rb_tree_iterator<_Value, _Ptr, _Ref> self;

		typedef _Rb_tree_node_base* Rb_tree_node_base_ptr;
		typedef _Rb_tree_node<value_type>* Rb_tree_node_ptr;


		_Rb_tree_iterator() : pNode() {}
		_Rb_tree_iterator(Rb_tree_node_base_ptr pBasePtr) : pNode(pBasePtr) {}
		_Rb_tree_iterator(Rb_tree_node_ptr pNodePtr) : pNode(pNodePtr) {}

		Rb_tree_node_base_ptr pNode;

		reference operator*()
		{
			return static_cast<Rb_tree_node_ptr>(pNode)->value;
		}

		pointer operator->()
		{
			return &(operator*());
		}

		self& operator++()
		{
			pNode = _Bs_tree<Rb_tree_node_base_ptr>::successor(pNode);
			return *this;
		}

		self operator++(int)
		{
			self _tmp = *this;
			pNode = _Bs_tree<Rb_tree_node_base_ptr>::successor(pNode);
			return _tmp;
		}

		self& operator--()
		{
			pNode = _Bs_tree<Rb_tree_node_base_ptr>::predecessor(pNode);
			return *this;
		}

		self operator--(int)
		{
			self _tmp = *this;
			pNode = _Bs_tree<Rb_tree_node_base_ptr>::predecessor(pNode);
			return _tmp;
		}

		self& operator=(self iter) {  
			pNode = iter.pNode; 
			return *this;
		}

		friend bool operator==(const self& first, const self& second) { return first.pNode == second.pNode; }
		friend bool operator!=(const self& first, const self& second) { return first.pNode != second.pNode; }
	};

	template <class _Key, class _Value, class _Compare, class _Alloc>
	class _Rb_tree
	{
	public:
		typedef _Key  key_type;
		typedef _Value value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef typename mtl::_Rb_tree_iterator<value_type, pointer, reference> iterator;
		typedef typename mtl::_Rb_tree_iterator<value_type, const_pointer, const_reference> const_iterator;
	protected:
		typedef _Rb_tree_node_base* base_ptr;
		typedef const _Rb_tree_node_base* const_base_ptr;

		typedef _Rb_tree_node<value_type>* node_ptr;
		typedef const _Rb_tree_node<value_type>* const_node_ptr;

		using node_alloc = typename _Alloc::template rebind<_Rb_tree_node<value_type>>::other;

	protected:
		node_ptr _get_node(const_reference value)
		{
			node_ptr pNode = m_node_allocator_.allocate(1);
			m_node_allocator_.construct(pNode, value);

			return pNode;
		}

		void _put_node(node_ptr pNode)
		{
			m_node_allocator_.destroy(pNode);
			m_node_allocator_.deallocate(pNode);
		}

		iterator _find(key_type& key)
		{
			node_ptr pNode = (node_ptr)_root();

			while (pNode)
			{
				if (m_compare_(key, pNode->value))
					pNode = pNode->left();
				else if (m_compare_(pNode->value, key))
					pNode = pNode->right();
				else
					return iterator(pNode);

			}

			return end();
		}

		base_ptr& _root() { return m_head_.pParent; }

		base_ptr& _left_most() { return m_head_.pLeft; }

		base_ptr& _right_most() { return m_head_.pRight; }

		base_ptr _uncle(base_ptr pNode)
		{
			if (pNode == _root())
				return 0;

			if (pNode->pParent == _root())
				return 0;

			base_ptr pParent = pNode->pParent;
			if (pParent == _root())
				return 0;

			
			return (pParent == pParent->pParent->pLeft) ?
				pParent->pParent->pRight :
				pParent->pParent->pLeft;
		}

		base_ptr _brother(base_ptr pNode)
		{
			if (pNode == _root())
				return 0;

			if (pNode->pParent->pLeft == pNode)
				return pNode->pParent->pRight;
			else
				return pNode->pParent->pLeft;
		}

		int _get_black_count(base_ptr pNode)
		{
			int nCount = 0;

			while (pNode != _root())
			{
				if (pNode->bColor == _Rb_tree_color::rbtcBlack)
					++nCount;
				pNode = pNode->pParent; 
			}
			return ++nCount;
		}

		void _rotate_left(base_ptr  pNode)
		{
			base_ptr pChild = pNode->pRight;
			pNode->pRight = pChild->pLeft;

			if (pChild->pLeft)
				pChild->pLeft->pParent = pNode;

			pChild->pParent = pNode->pParent;

			if (_root() == pNode)
				_root() = pChild;
			else if (pNode == pNode->pParent->pLeft)
				pNode->pParent->pLeft = pChild;
			else //  pNode == pNode->pParent->pRight
				pNode->pParent->pRight = pChild;

			pNode->pParent = pChild;
			pChild->pLeft = pNode;
		}

		void _rotate_right(base_ptr pNode)
		{
			base_ptr pChild = pNode->pLeft;
			pNode->pLeft = pChild->pRight;

			if (pChild->pRight)
				pChild->pRight->pParent = pNode;

			pChild->pParent = pNode->pParent;

			if (_root() == pNode)
				_root() = pChild;
			else if (pNode == pNode->pParent->pLeft)
				pNode->pParent->pLeft = pChild;
			else // pNode == pNode->pParent->pRight
				pNode->pParent->pRight = pChild;

			pNode->pParent = pChild;
			pChild->pRight = pNode;
		}

		void _rebalance(base_ptr pNode)
		{
			base_ptr pUncle = 0;
			base_ptr pGrandParent = 0;
			base_ptr pParent = 0;
			pNode->color() = _Rb_tree_color::rbtcRed;

			while (_root() != pNode && pNode->pParent->color() == _Rb_tree_color::rbtcRed)
			{
				pUncle = _uncle(pNode);
				pParent = pNode->pParent;
				pGrandParent = pParent->pParent;

				if (pUncle && pUncle->color() == _Rb_tree_color::rbtcRed)
				{
					pGrandParent->color() = _Rb_tree_color::rbtcRed;
					pParent->color() = _Rb_tree_color::rbtcBlack;
					pUncle->color() = _Rb_tree_color::rbtcBlack;
					pNode = pGrandParent;
					continue;
				}

				if (pParent == pGrandParent->pLeft)
				{
					if (pNode == pParent->pRight)
					{
						// internal rotation
						pNode = pNode->pParent;
						_rotate_left(pNode);
						pParent = pNode->pParent;
					}

					// TODO: outer rotation
					pGrandParent->bColor = _Rb_tree_color::rbtcRed;
					pParent->bColor = _Rb_tree_color::rbtcBlack;
					_rotate_right(pGrandParent);
				}
				else // pParent == pGrandParent->pRight
				{
					if (pNode == pParent->pLeft)
					{
						pNode = pNode->pParent;
						_rotate_right(pParent);
						pParent = pNode->pParent;
					}

					pGrandParent->bColor = _Rb_tree_color::rbtcRed;
					pParent->bColor = _Rb_tree_color::rbtcBlack;
					_rotate_left(pGrandParent);
				}

			}

			_root()->color() = _Rb_tree_color::rbtcBlack;
		}

		void _rebalance_for_erase(base_ptr pNode, base_ptr pParent)
		{
			base_ptr pBrother = 0;

			// child is not the red node,Can't handler it myself
			while ((!pNode || pNode->bColor == _Rb_tree_color::rbtcBlack) &&
				pNode != _root())
			{
				if (pParent->pLeft == pNode)
				{
					pBrother = pParent->pRight;
					
					// this node is not brother, to find real brother
					if (pBrother->bColor == _Rb_tree_color::rbtcRed)
					{
						_rotate_left(pParent);
						pParent->bColor = _Rb_tree_color::rbtcRed;
						pBrother->bColor = _Rb_tree_color::rbtcBlack;
						pBrother = pParent->pRight;
					}

					// brother any child is red node, Can handle
					if ((pBrother->pLeft && pBrother->pLeft->bColor == _Rb_tree_color::rbtcRed) ||
						pBrother->pRight && pBrother->pRight->bColor == _Rb_tree_color::rbtcRed)
					{
						// left child is red node
						if (pBrother->pLeft && pBrother->pLeft->bColor == _Rb_tree_color::rbtcRed)
						{
							_rotate_right(pBrother);
							pBrother = pBrother->pParent;
							pBrother->bColor = _Rb_tree_color::rbtcBlack;
						}
						else
							pBrother->pRight->bColor = _Rb_tree_color::rbtcBlack;

						pBrother->bColor = pParent->bColor;
						pParent->bColor = _Rb_tree_color::rbtcBlack;
						_rotate_left(pParent);
						break; // complete, any path black node same
					}
					else
					{
						pBrother->bColor = _Rb_tree_color::rbtcRed;
						pNode = pParent;
						pParent = pNode->pParent;
					}
				}
				else // pParent->pRight == pNode
				{
					pBrother = pParent->pLeft;
					
					if (pBrother->bColor == _Rb_tree_color::rbtcRed)
					{
						_rotate_right(pParent);
						pParent->bColor = _Rb_tree_color::rbtcRed;
						pBrother->bColor = _Rb_tree_color::rbtcBlack;
						pBrother = pParent->pLeft;
					}

					if ((pBrother->pLeft && pBrother->pLeft->bColor == _Rb_tree_color::rbtcRed) ||
						pBrother->pRight && pBrother->pRight->bColor == _Rb_tree_color::rbtcRed)
					{
						if (pBrother->pRight && pBrother->pRight->bColor == _Rb_tree_color::rbtcRed)
						{
							_rotate_left(pBrother);
							pBrother = pBrother->pParent;
							pBrother->bColor = _Rb_tree_color::rbtcBlack;
						}
						else
							pBrother->pLeft->bColor = _Rb_tree_color::rbtcBlack;

						pBrother->bColor = pParent->bColor;
						pParent->bColor = _Rb_tree_color::rbtcBlack;
						_rotate_right(pParent);
						break; // complete, any path black node same
					}
					else
					{
						pBrother->bColor = _Rb_tree_color::rbtcRed;
						pNode = pParent;
						pParent = pNode->pParent;
					}
				}
			}

			if (pNode)
				pNode->bColor = _Rb_tree_color::rbtcBlack;
		}

		iterator _insert(node_ptr& pInsertPoint, node_ptr pParent, const_reference value)
		{
			node_ptr pNode = _get_node(value);

			// empty tree
			if (_root() == 0)
			{
				// this is tree header
				_root() = pNode;
				_left_most() = pNode;
				_right_most() = pNode;
			}
			else
			{
				pInsertPoint = pNode;

				if (_left_most() == pParent)
				{
					if (pInsertPoint == pParent->left())
						_left_most() = pInsertPoint;
				}
				else
				{
					if (pInsertPoint == pParent->right())
						_right_most() = pInsertPoint;
				}
			}

			pNode->pParent = pParent;

			_rebalance(pNode);
			++m_size_;

			if (!_verify())
				throw 1;

			return iterator(pNode);
		}

		void _erase(node_ptr pNode)
		{
			while (pNode != 0)													    
			{
				_erase(pNode->right());
				node_ptr pY = pNode->left();
				_put_node(pNode);
				pNode = pY;
			}
		}

		void _reset()
		{
			m_head_.pParent = 0;
			m_head_.pLeft = &m_head_;
			m_head_.pRight = &m_head_;
			m_size_ = 0;
		}

		void _erase_aux(base_ptr pNode)
		{
			base_ptr pParent = pNode->pParent;
			base_ptr pChild = 0;

			if (pNode->pLeft != 0 && pNode->pRight != 0)
			{
				// left child of pSuccessor must be 0
				base_ptr pSuccessor = _Bs_tree<base_ptr>::successor(pNode);
				pChild = pSuccessor->pRight;

				// case 1: successor as pNode right child
				if (pSuccessor == pNode->pRight)
				{
					pSuccessor->pLeft = pNode->pLeft;
					pNode->pLeft->pParent = pSuccessor;
					pParent = pSuccessor;

					_Bs_tree<base_ptr>::transplant(pNode, pSuccessor, _root());
				}
				else // case 2: successor replace pNode, delete successor
				{
					pParent = pSuccessor->pParent;

					pSuccessor->pLeft = pNode->pLeft;
					pSuccessor->pRight = pNode->pRight;
					pNode->pLeft->pParent = pSuccessor;
					pNode->pRight->pParent = pSuccessor;


					_Bs_tree<base_ptr>::transplant(pSuccessor, pChild, _root());
					_Bs_tree<base_ptr>::transplant(pNode, pSuccessor, _root());
				}

				std::swap<_Rb_tree_color>(pSuccessor->bColor, pNode->bColor);
			}
			else
			{
				// case 3: left  child is 0
				// case 4: right child is 0
				// case 5: left and right is 0
				if (pNode->pLeft == 0)
					pChild = pNode->pRight;
				else // pNode->pRight == 0 or pLeft and pRight is 0
					pChild = pNode->pLeft;

				_Bs_tree<base_ptr>::transplant(pNode, pChild, _root());

				if (!_root())
					_left_most() = _right_most() = &m_head_;
				else if(pNode == _left_most())
					_left_most() = _Bs_tree<base_ptr>::min_num(_root());
				else if(pNode == _right_most())
					_right_most() = _Bs_tree<base_ptr>::max_num(_root());
			}

			if (pNode->bColor == _Rb_tree_color::rbtcBlack)
				_rebalance_for_erase(pChild, pParent);

			--m_size_;
			_put_node((node_ptr)pNode);
		}

		void _erase(const_iterator first, const_iterator last)
		{
			if (first == begin() && last == end())
				clear();
			else
			{
				while (first != end())
				{
					_erase_aux(first.pNode);
					first++;
				}
			}
		}

		bool _verify()
		{
			// check header
			if (m_size_ == 0)
			{
				return m_head_.pLeft == m_head_.pRight &&
					m_head_.pLeft == &m_head_ &&
					begin() == end() &&
					_left_most() == _right_most();
			}

			node_ptr pMiniNum = 0;
			int nLeftBlackCount = _get_black_count(begin().pNode);

			for (iterator iter = begin(); iter != end(); ++iter)
			{
				node_ptr pNode = (node_ptr)iter.pNode;

				if (pNode->bColor == _Rb_tree_color::rbtcRed)
				{
					if (pNode->left() && pNode->left()->bColor == _Rb_tree_color::rbtcRed)
						return false;
					if (pNode->right() && pNode->right()->bColor == _Rb_tree_color::rbtcRed)
						return false;
				}

				if (!pMiniNum || m_compare_(pMiniNum->value, pNode->value))
					pMiniNum = pNode;
				else // !m_compare_(pMiniNum, pNode)
					return false;

				if (!pNode->pLeft && !pNode->pRight)
				{
					if (nLeftBlackCount != _get_black_count(iter.pNode))
						return false;
				}
			}

			return true;
		}

	public:
		_Rb_tree()
		{
			_reset();
		}

		~_Rb_tree() { clear(); }


		void print_tree()
		{
			CPrintBTree<node_ptr>::Print((node_ptr)_root());
			cout << "size:" << m_size_ << endl;
		}

		iterator begin() { return iterator(_left_most()); }
		iterator end() { return iterator(&m_head_); }
		size_type size() const { return m_size_; }
		iterator find(key_type& key) { return _find(key); }
		void clear()
		{
			_erase((node_ptr)_root());
			_reset();
		}
		void erase(iterator iter)
		{
			int nValue = iter->first;

			std::cout << "remove:" << nValue << "\n";

			_erase_aux(iter.pNode);

			if (!_verify())
				throw 1;
		}
		iterator insert_unique(const_reference value)
		{
			node_ptr pX = (node_ptr)_root();
			node_ptr pParent = (node_ptr)&m_head_;

			bool bComp = true;

			while (pX)
			{
				pParent = pX;
				bComp = m_compare_(value, pX->value);
				pX = bComp ? pX->left() : pX->right();
			}

			iterator iter = iterator(pParent);
			if (bComp)
			{
				if (iter == begin())
					return _insert(pParent->left(), pParent, value);
				else
					--iter;
			}

			if (m_compare_(*iter, value))
				return _insert(bComp ? pParent->left() : pParent->right(), pParent, value);

			return end();
		}
		iterator insert_equal(const_reference value)
		{
			node_ptr pParent = &m_head_;
			node_ptr pInsertPoint = (node_ptr)_root();
			bool bComp = true;
			while (pInsertPoint)
			{
				pParent = pInsertPoint;
				pInsertPoint = m_compare_(value, pInsertPoint->value) ?
					pInsertPoint->pLeft :
					pInsertPoint->pRight;
			}

			return _insert(bComp ? pParent->left() : pParent->right(), pParent, value);
		}

	private:
		_Compare m_compare_;
		_Rb_tree_node_base m_head_;
		node_alloc m_node_allocator_;
		size_type m_size_;
	};
}

#endif // _H_RB_TREE_H_