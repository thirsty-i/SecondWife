#pragma once

#include "mtl/struct/ring_buffer.h"

namespace mtl
{
	template <class T>
	class circle_list final
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

		circle_list(const size_t size, memory_resource* resource = get_new_delete_resource())
			: ring_buffer_(size, resource)
		{};

		bool push(const_reference data)
		{
			if (full())
				return false;

			ring_buffer_.write(data, _data_size());
		}

		bool pop(reference data)
		{
			if (empty())
				return false;

			ring_buffer_.read(data, _data_size());
			return true;
		}

		bool empty() { return !get_used(); }
		bool full() { return get_unused() < _data_size(); }
	private:
		constexpr size_type _data_size() { return sizeof(T); }
	private:
		ring_buffer ring_buffer_;
	};
};