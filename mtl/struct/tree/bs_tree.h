#pragma once

namespace mtl
{
	template <class _NodePtr>
	class _Bs_tree
	{
	public:
		typedef _NodePtr pointer;
		typedef const _NodePtr const_pointer;

		//_Bs_tree() = delete;
		//_Bs_tree(pointer pHead) : m_Head(pHead) {};

		static pointer predecessor(const_pointer cnode)
		{
			if (cnode == 0)
				return 0;

			pointer node = cnode;

			if (node->left)
			{
				node = node->left;

				while (node->left)
					node = node->left;
			}
			else
			{
				while (node->parent->left == node)
					node = node->parent;

				if (node->left != node->parent)
					node = node->parent;
			}

			return node;
		}

		static pointer successor(const_pointer cnode)
		{
			if (cnode == 0)
				return 0;

			pointer node = cnode;

			if (node->right)
			{
				node = node->right;

				while (node->left)
					node = node->left;
			}
			else
			{
				while (node->parent->right == node)
					node = node->parent;

				if (node->right != node->parent)
					node = node->parent;
			}

			return node;
		}

		static pointer left_most(const_pointer node)
		{
			if (node == 0)
				return 0;

			pointer tmp = node;
			while (tmp->left)
				tmp = tmp->left;
			return tmp;
		}

		static pointer right_most(const_pointer node)
		{
			if (node == 0)
				return 0;

			pointer tmp = node;
			while (tmp->right)
				tmp = tmp->right;
			return tmp;
		}
	
		static void transplant(pointer pDst, pointer pSrc, pointer& pRoot)
		{
			const_pointer parent = pDst->parent;
			if (pDst == pRoot)
			{
				parent->parent = pSrc;
				pRoot = pSrc;
			}
			else
			{
				if (parent->left == pDst)
					parent->left = pSrc;
				else // parent->right == pDst
					parent->right = pSrc;
			}

			if (pSrc)
				pSrc->parent = parent;
		}
	private:
		//pointer m_Head;
	};
}