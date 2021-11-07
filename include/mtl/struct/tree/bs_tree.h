#ifndef _H_BINARY_TREE_H_
#define _H_BINARY_TREE_H_

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

		static pointer predecessor(const_pointer pcNode)
		{
			if (pcNode == 0)
				return 0;

			pointer pNode = pcNode;

			if (pNode->pLeft)
			{
				pNode = pNode->pLeft;

				while (pNode->pLeft)
					pNode = pNode->pLeft;
			}
			else
			{
				while (pNode->pParent->pLeft == pNode)
					pNode = pNode->pParent;

				if (pNode->pLeft != pNode->pParent)
					pNode = pNode->pParent;
			}

			return pNode;
		}

		static pointer successor(const_pointer pcNode)
		{
			if (pcNode == 0)
				return 0;

			pointer pNode = pcNode;

			if (pNode->pRight)
			{
				pNode = pNode->pRight;

				while (pNode->pLeft)
					pNode = pNode->pLeft;
			}
			else
			{
				while (pNode->pParent->pRight == pNode)
					pNode = pNode->pParent;

				if (pNode->pRight != pNode->pParent)
					pNode = pNode->pParent;
			}

			return pNode;
		}

		static pointer leftMost(const_pointer pNode)
		{
			if (pNode == 0)
				return 0;

			pointer _pTmp = pNode;
			while (_pTmp->pLeft)
				_pTmp = _pTmp->pLeft;
			return _pTmp;
		}

		static pointer right_most(const_pointer pNode)
		{
			if (pNode == 0)
				return 0;

			pointer _pTmp = pNode;
			while (_pTmp->pRight)
				_pTmp = _pTmp->pRight;
			return _pTmp;
		}
	
		static void transplant(pointer pDst, pointer pSrc, pointer& pRoot)
		{
			const_pointer pParent = pDst->pParent;
			if (pDst == pRoot)
			{
				pParent->pParent = pSrc;
				pRoot = pSrc;
			}
			else
			{
				if (pParent->pLeft == pDst)
					pParent->pLeft = pSrc;
				else // pParent->pRight == pDst
					pParent->pRight = pSrc;
			}

			if (pSrc)
				pSrc->pParent = pParent;
		}
	
		static pointer min_num(const_pointer pRoot)
		{
			if (pRoot == 0)
				return 0;

			pointer pNode = pRoot;
			while (pNode->pLeft)
				pNode = pNode->pLeft;

			return pNode;
		}

		static pointer max_num(const_pointer pRoot)
		{
			if (pRoot == 0)
				return 0;

			pointer pNode = pRoot;
			while (pNode->pRight)
				pNode = pNode->pRight;

			return pNode;
		}
	private:
		//pointer m_Head;
	};
}
#endif 	// _H_BINARY_TREE_H_