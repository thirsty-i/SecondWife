#ifndef _H_OBJECT_POOL_H_
#define	_H_OBJECT_POOL_H_

#include "type_int.h"
#include "allocator.h"
#include "stack.h"
#include <deque>
#include <utility>
#include "lock.h"
#include <list>

namespace mtl
{
	// 这里暂时用特化解决，目前没有找到STL的模板三目运算
	template <bool Lock>
	struct LockTraits
	{
		using lock_type = CNullLock;
	};

	template <>
	struct LockTraits<true>
	{
		using lock_type = CSpinLock;
	};

	template <class T, bool Lock, class Alloc = allocator<T>>
	class CObjectPool
	{
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using size_type = size_t;
		using allocator_type = typename Alloc::template rebind<value_type>::other;
		using lock_type = typename LockTraits<Lock>::lock_type;

		explicit CObjectPool(size_type size) : m_Size(size) {};
		~CObjectPool() {
			Clear();
			// TODO: check allocate.count == deallocate.cout?
		};

		CObjectPool& operator=(CObjectPool&) = delete;
		CObjectPool(const CObjectPool&) = delete;				 
		CObjectPool() = delete;

	protected:
		pointer _Refill()
		{
			size_type refillSize = _GetRefillSize();
			pointer pChunk = m_Allocator.allocate(refillSize);
			m_Size += refillSize;

			while (--refillSize)
				m_FreeStack.Push(pChunk + refillSize);

			m_lstChunk.push_back(pChunk);
			return pChunk; // return object memory
		}

		size_type _GetRefillSize() { return m_Size; }

	public:
		void Clear()
		{
			while (!m_lstChunk.empty())
			{
				m_Allocator.deallocate(m_lstChunk.front());
				m_lstChunk.pop_front();
			}
		}

		template <class... Args>
		pointer New(Args&&... args)
		{
			CLockGuard<lock_type> oLockGuard(m_Lock);
			pointer pNode = 0;
			if (m_FreeStack.Empty())
				pNode = _Refill();
			else
			{
				pNode = m_FreeStack.Top();
				m_FreeStack.Pop();
			}

			if (pNode)
				m_Allocator.construct(pNode, std::forward<Args>(args)...);

			return pNode;
		}

		void Delete(pointer pNode)
		{
			CLockGuard<lock_type> oLockGuard(m_Lock);
			m_Allocator.destroy(pNode);
			m_FreeStack.Push(pNode);
		}
	protected:
		std::list<pointer> m_lstChunk;
		CStack<pointer>  m_FreeStack;
		allocator_type m_Allocator;	  // 改成指针?
		size_type m_Size;
		lock_type m_Lock;
	};
};
#endif // _H_OBJECT_POOL_H_