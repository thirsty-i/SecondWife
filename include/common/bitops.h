#ifndef _H_BIT_OPS_H_
#define _H_BIT_OPS_H_

#include "type_int.h"
#include <type_traits>

template <class Integer>
static UINT32 fls(Integer x)
{
	static_assert(std::is_integral<Integer>::value, "param not integral");

	if (!x)
		return 0;

	INT8 highest = sizeof(Integer) * 8;
	Integer _bit = highest;
	Integer _tmp = -1;

	while (_bit > 1)
	{
		_bit >>= 1;
		_tmp <<= _bit;

		if (!(x & _tmp)) {
			x <<= _bit;
			highest -= _bit;
		}
	}

	return highest;
}


template <class Integer>
static bool Is2Pow(Integer x)
{
	static_assert(std::is_integral<Integer>::value, "param not integral");
	return x && ((x & (x - 1)) == 0);
}

template <class Integer>
static Integer RoundupPowOf2(Integer x)
{
	static_assert(std::is_integral<Integer>::value, "param not integral");
	return (x ? 1 << fls<Integer>(x - 1) : 0);
}

template <class Integer>
static Integer RounddownPowOf2(Integer x)
{
	static_assert(std::is_integral<Integer>::value, "param not integral");
	return (x ? 1 << (fls<Integer>(x) - 1): 0);
}

#endif // _H_BIT_OPS_H_