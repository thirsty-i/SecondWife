#ifndef _H_CIRCLE_LIST_H_
#define _H_CIRCLE_LIST_H_

#include "mtl/struct/ring_buffer.h"

namespace mtl
{
	template <class T, size_t Size>
	class circle_list
		: private ring_buffer
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = size_t;

		/*
		*  not 2^
		*/
		static_assert(is_2_pow(sizeof(value_type)), "circle_list value not 2 pow");
		static_assert(is_2_pow(Size), "circle_list size not 2 pow");
		
		static_assert(std::is_pod<value_type>, "T is not POD");

		circle_list()
			: ring_buffer(datas, sizeof(datas))
		{};

		bool push(const_reference data)
		{
			if (full())
				return false;

			write(data, _data_size());
		}

		bool pop(reference data)
		{
			if (empty())
				return false;

			read(data, _data_size());
			return true;
		}

		bool empty() { return !get_used(); }
		bool full() { return get_unused() < _data_size(); }
	private:
		constexpr size_type _data_size() { return sizeof(T); }
	private:
		value_type datas[Size];
	};
};

#endif // _H_CIRCLE_LIST_H_