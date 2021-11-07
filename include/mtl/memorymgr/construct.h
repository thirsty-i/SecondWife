#ifndef _H_CONSTRUCT_H_
#define _H_CONSTRUCT_H_

#include <new>
#include <utility>

template <class T, class... Args>
void construct(T* __p, Args&&... args)
{
	::new ((void*)__p) T(std::forward<Args>(args)...);
}

template <class T>
void destroy(T* __p)
{
	__p->~T();
}

#endif // _H_CONSTRUCT_H_