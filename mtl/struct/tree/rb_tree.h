#pragma once

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
			parent = left = right = 0;
			color = _Rb_tree_color::rbtcRed;
		}

		void clone(node_base_ptr node)
		{
			node->parent = parent;
			node->left = left;
			node->right = right;
			node->color = color;
		}

		node_base_ptr parent;
		node_base_ptr left;
		node_base_ptr right;
		_Rb_tree_color color;
	};

	template <typename _Value>
	struct _Rb_tree_node : public _Rb_tree_node_base
	{
		typedef _Value value_type;
		typedef _Rb_tree_node<value_type>* node_ptr;
		
		_Rb_tree_node(const value_type& value) : _Rb_tree_node_base(), value(value) {}
		_Rb_tree_node(value_type& value) : _Rb_tree_node_base(), value(value) {}
		_Rb_tree_node(value_type&& value) : _Rb_tree_node_base(), value(value) {}

		node_ptr& node_left()
		{
			return (node_ptr&)left;
		}

		node_ptr& node_right()
		{
			return (node_ptr&)right;
		}

		node_ptr& node_parent()
		{
			return (node_ptr&)parent;
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


		_Rb_tree_iterator() : node() {}
		_Rb_tree_iterator(Rb_tree_node_base_ptr pBasePtr) : node(pBasePtr) {}
		_Rb_tree_iterator(Rb_tree_node_ptr pNodePtr) : node(pNodePtr) {}

		Rb_tree_node_base_ptr node;

		reference operator*()
		{
			return static_cast<Rb_tree_node_ptr>(node)->value;
		}

		pointer operator->()
		{
			return &(operator*());
		}

		self& operator++()
		{
			node = _Bs_tree<Rb_tree_node_base_ptr>::successor(node);
			return *this;
		}

		self operator++(int)
		{
			self _tmp = *this;
			++* this;
			return _tmp;
		}

		self& operator--()
		{
			node = _Bs_tree<Rb_tree_node_base_ptr>::predecessor(node);
			return *this;
		}

		self operator--(int)
		{
			self _tmp = *this;
			--* this;
			return _tmp;
		}

		self& operator=(self iter) {  
			node = iter.node; 
			return *this;
		}

		friend bool operator==(const self& first, const self& second) { return first.node == second.node; }
		friend bool operator!=(const self& first, const self& second) { return first.node != second.node; }
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
			node_ptr node = node_allocator_.allocate(1);
			node_allocator_.construct(node, value);

			return node;
		}

		void _put_node(node_ptr node)
		{
			node_allocator_.destroy(node);
			node_allocator_.deallocate(node, 1);
		}

		iterator _find(key_type& key)
		{
			node_ptr node = (node_ptr)_root();

			while (node)
			{
				if (compare_(key, node->value))
					node = node->node_left();
				else if (compare_(node->value, key))
					node = node->node_right();
				else
					return iterator(node);
			}

			return end();
		}

		base_ptr& _root() { return head_.parent; }

		base_ptr& _left_most() { return head_.left; }

		base_ptr& _right_most() { return head_.right; }

		base_ptr _uncle(base_ptr node)
		{
			if (node == _root())
				return 0;

			if (node->parent == _root())
				return 0;

			base_ptr parent = node->parent;
			if (parent == _root())
				return 0;

			
			return (parent == parent->parent->left) ?
				parent->parent->right :
				parent->parent->left;
		}

		base_ptr _brother(base_ptr node)
		{
			if (node == _root())
				return 0;

			if (node->parent->left == node)
				return node->parent->right;
			else
				return node->parent->left;
		}

		int _get_black_count(base_ptr node)
		{
			int count = 0;

			while (node != _root())
			{
				if (node->color == _Rb_tree_color::rbtcBlack)
					++count;
				node = node->parent; 
			}
			return ++count;
		}

		void _rotate_left(base_ptr  node)
		{
			base_ptr child = node->right;
			node->right = child->left;

			if (child->left)
				child->left->parent = node;

			child->parent = node->parent;

			if (_root() == node)
				_root() = child;
			else if (node == node->parent->left)
				node->parent->left = child;
			else //  node == node->parent->pRight
				node->parent->right = child;

			node->parent = child;
			child->left = node;
		}

		void _rotate_right(base_ptr node)
		{
			base_ptr child = node->left;
			node->left = child->right;

			if (child->right)
				child->right->parent = node;

			child->parent = node->parent;

			if (_root() == node)
				_root() = child;
			else if (node == node->parent->left)
				node->parent->left = child;
			else // node == node->parent->pRight
				node->parent->right = child;

			node->parent = child;
			child->right = node;
		}

		void _rebalance(base_ptr node)
		{
			base_ptr uncle = 0;
			base_ptr grand_parent = 0;
			base_ptr parent = 0;
			node->color = _Rb_tree_color::rbtcRed;

			while (_root() != node && node->parent->color == _Rb_tree_color::rbtcRed)
			{
				uncle = _uncle(node);
				parent = node->parent;
				grand_parent = parent->parent;

				if (uncle && uncle->color == _Rb_tree_color::rbtcRed)
				{
					grand_parent->color = _Rb_tree_color::rbtcRed;
					parent->color = _Rb_tree_color::rbtcBlack;
					uncle->color = _Rb_tree_color::rbtcBlack;
					node = grand_parent;
					continue;
				}

				if (parent == grand_parent->left)
				{
					if (node == parent->right)
					{
						// internal rotation
						node = node->parent;
						_rotate_left(node);
						parent = node->parent;
					}

					// TODO: outer rotation
					grand_parent->color = _Rb_tree_color::rbtcRed;
					parent->color = _Rb_tree_color::rbtcBlack;
					_rotate_right(grand_parent);
				}
				else // parent == grand_parent->pRight
				{
					if (node == parent->left)
					{
						node = node->parent;
						_rotate_right(parent);
						parent = node->parent;
					}

					grand_parent->color = _Rb_tree_color::rbtcRed;
					parent->color = _Rb_tree_color::rbtcBlack;
					_rotate_left(grand_parent);
				}

			}

			_root()->color = _Rb_tree_color::rbtcBlack;
		}

		void _rebalance_for_erase(base_ptr node, base_ptr parent)
		{
			base_ptr brother = 0;

			// child is not the red node,Can't handler it myself
			while ((!node || node->color == _Rb_tree_color::rbtcBlack) &&
				node != _root())
			{
				if (parent->left == node)
				{
					brother = parent->right;
					
					// this node is not brother, to find real brother
					if (brother->color == _Rb_tree_color::rbtcRed)
					{
						_rotate_left(parent);
						parent->color = _Rb_tree_color::rbtcRed;
						brother->color = _Rb_tree_color::rbtcBlack;
						brother = parent->right;
					}

					// brother any child is red node, Can handle
					if ((brother->left && brother->left->color == _Rb_tree_color::rbtcRed) ||
						brother->right && brother->right->color == _Rb_tree_color::rbtcRed)
					{
						// left child is red node
						if (brother->left && brother->left->color == _Rb_tree_color::rbtcRed)
						{
							_rotate_right(brother);
							brother = brother->parent;
							brother->color = _Rb_tree_color::rbtcBlack;
						}
						else
							brother->right->color = _Rb_tree_color::rbtcBlack;

						brother->color = parent->color;
						parent->color = _Rb_tree_color::rbtcBlack;
						_rotate_left(parent);
						break; // complete, any path black node same
					}
					else
					{
						brother->color = _Rb_tree_color::rbtcRed;
						node = parent;
						parent = node->parent;
					}
				}
				else // parent->pRight == node
				{
					brother = parent->left;
					
					if (brother->color == _Rb_tree_color::rbtcRed)
					{
						_rotate_right(parent);
						parent->color = _Rb_tree_color::rbtcRed;
						brother->color = _Rb_tree_color::rbtcBlack;
						brother = parent->left;
					}

					if ((brother->left && brother->left->color == _Rb_tree_color::rbtcRed) ||
						brother->right && brother->right->color == _Rb_tree_color::rbtcRed)
					{
						if (brother->right && brother->right->color == _Rb_tree_color::rbtcRed)
						{
							_rotate_left(brother);
							brother = brother->parent;
							brother->color = _Rb_tree_color::rbtcBlack;
						}
						else
							brother->left->color = _Rb_tree_color::rbtcBlack;

						brother->color = parent->color;
						parent->color = _Rb_tree_color::rbtcBlack;
						_rotate_right(parent);
						break; // complete, any path black node same
					}
					else
					{
						brother->color = _Rb_tree_color::rbtcRed;
						node = parent;
						parent = node->parent;
					}
				}
			}

			if (node)
				node->color = _Rb_tree_color::rbtcBlack;
		}

		iterator _insert(node_ptr& insert_point, node_ptr parent, const_reference value)
		{
			node_ptr node = _get_node(value);

			// empty tree
			if (_root() == 0)
			{
				// this is tree header
				_root() = node;
				_left_most() = node;
				_right_most() = node;
			}
			else
			{
				insert_point = node;

				if (_left_most() == parent)
				{
					if (insert_point == parent->node_left())
						_left_most() = insert_point;
				}
				else
				{
					if (insert_point == parent->node_right())
						_right_most() = insert_point;
				}
			}

			node->parent = parent;

			_rebalance(node);
			++size_;

			if (!_verify())
				throw 1;

			return iterator(node);
		}

		void _erase(node_ptr node)
		{
			while (node != 0)													    
			{
				_erase(node->node_right());
				node_ptr y = node->node_left();
				_put_node(node);
				node = y;
			}
		}

		void _reset()
		{
			head_.parent = 0;
			head_.left = &head_;
			head_.right = &head_;
			size_ = 0;
		}

		void _erase_aux(base_ptr node)
		{
			base_ptr parent = node->parent;
			base_ptr child = 0;

			if (node->left != 0 && node->right != 0)
			{
				// left child of pSuccessor must be 0
				base_ptr pSuccessor = _Bs_tree<base_ptr>::successor(node);
				child = pSuccessor->right;

				// case 1: successor as node right child
				if (pSuccessor == node->right)
				{
					pSuccessor->left = node->left;
					node->left->parent = pSuccessor;
					parent = pSuccessor;

					_Bs_tree<base_ptr>::transplant(node, pSuccessor, _root());
				}
				else // case 2: successor replace node, delete successor
				{
					parent = pSuccessor->parent;

					pSuccessor->left = node->left;
					pSuccessor->right = node->right;
					node->left->parent = pSuccessor;
					node->right->parent = pSuccessor;


					_Bs_tree<base_ptr>::transplant(pSuccessor, child, _root());
					_Bs_tree<base_ptr>::transplant(node, pSuccessor, _root());
				}

				std::swap<_Rb_tree_color>(pSuccessor->color, node->color);
			}
			else
			{
				// case 3: left  child is 0
				// case 4: right child is 0
				// case 5: left and right is 0
				if (node->left == 0)
					child = node->right;
				else // node->pRight == 0 or pLeft and pRight is 0
					child = node->left;

				_Bs_tree<base_ptr>::transplant(node, child, _root());

				if (!_root())
					_left_most() = _right_most() = &head_;
				else if(node == _left_most())
					_left_most() = _Bs_tree<base_ptr>::left_most(_root());
				else if(node == _right_most())
					_right_most() = _Bs_tree<base_ptr>::right_most(_root());
			}

			if (node->color == _Rb_tree_color::rbtcBlack)
				_rebalance_for_erase(child, parent);

			--size_;
			_put_node((node_ptr)node);
		}

		void _erase(const_iterator first, const_iterator last)
		{
			if (first == begin() && last == end())
				clear();
			else
			{
				while (first != end())
				{
					_erase_aux(first.node);
					first++;
				}
			}
		}

		bool _verify()
		{
			// check header
			if (size_ == 0)
			{
				return head_.left == head_.right &&
					head_.left == &head_ &&
					begin() == end() &&
					_left_most() == _right_most();
			}

			node_ptr pMiniNum = 0;
			int nLeftBlackCount = _get_black_count(begin().node);

			for (iterator iter = begin(); iter != end(); ++iter)
			{
				node_ptr node = (node_ptr)iter.node;

				if (node->color == _Rb_tree_color::rbtcRed)
				{
					if (node->node_left() && node->node_left()->color == _Rb_tree_color::rbtcRed)
						return false;
					if (node->node_right() && node->node_right()->color == _Rb_tree_color::rbtcRed)
						return false;
				}

				if (!pMiniNum || compare_(pMiniNum->value, node->value))
					pMiniNum = node;
				else if(compare_(node->value, pMiniNum->value)) // inorder next node > current node
					return false;

				// left
				if (!node->left && !node->right)
				{
					if (nLeftBlackCount != _get_black_count(iter.node))
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
			CPrintBTree<node_ptr>::print((node_ptr)_root());
			cout << "size:" << size_ << endl;
		}

		iterator begin() { return iterator(_left_most()); }
		iterator end() { return iterator(&head_); }
		size_type size() const { return size_; }
		iterator find(key_type& key) { return _find(key); }
		void clear()
		{
			_erase((node_ptr)_root());
			_reset();
		}
		void erase(iterator iter)
		{
			int value = iter->first;

			std::cout << "remove:" << value << "\n";

			_erase_aux(iter.node);

			if (!_verify())
				throw 1;
		}
		iterator insert_unique(const_reference value)
		{
			node_ptr pX = (node_ptr)_root();
			node_ptr parent = (node_ptr)&head_;

			bool comp = true;

			while (pX)
			{
				parent = pX;
				comp = compare_(value, pX->value);
				pX = comp ? pX->node_left() : pX->node_right();
			}

			iterator iter = iterator(parent);
			if (comp)
			{
				if (iter == begin())
					return _insert(parent->node_left(), parent, value);
				else
					--iter;
			}

			if (compare_(*iter, value))
				return _insert(comp ? parent->node_left() : parent->node_right(), parent, value);

			return end();
		}
		iterator insert_equal(const_reference value)
		{
			node_ptr parent = (node_ptr)&head_;
			node_ptr insert_point = (node_ptr)_root();
			bool comp = true;
			while (insert_point)
			{
				parent = insert_point;
				comp = compare_(value, insert_point->value);
				insert_point = comp
					? insert_point->node_left()
					: insert_point->node_right();
			}

			return _insert(comp ? parent->node_left() : parent->node_right(), parent, value);
		}

	private:
		_Compare compare_;
		_Rb_tree_node_base head_;
		node_alloc node_allocator_;
		size_type size_;
	};
}