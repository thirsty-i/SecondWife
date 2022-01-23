#pragma once

#include "common/node.h"

template <class T>
class free_list
{
public:
	using node_t = forward_node;

	template <class T>
	void push(T* ptr)
	{
	
	}

	template <class T>
	T* pop()
	{
	
	}

private:
	void* header_;
};