#pragma once

#include <type_traits>
#include <cstdint>

template <class integer>
static integer fls(integer x)
{
	static_assert(std::is_integral<integer>::value, "param not integral");

	if (!x)
		return 0;

	integer highest = sizeof(integer) * 8;
	integer bit = highest;
	integer tmp = -1;

	while (bit > 1)
	{
		bit >>= 1;
		tmp <<= bit;

		if (!(x & tmp)) {
			x <<= bit;
			highest -= bit;
		}
	}

	return highest;
}


template <class integer>
static bool is_2_pow(integer x)
{
	static_assert(std::is_integral<integer>::value, "param not integral");
	return x && ((x & (x - 1)) == 0);
}

template <class integer>
static integer round_up_pow_of_2(integer x)
{
	static_assert(std::is_integral<integer>::value, "param not integral");
	return (x ? 1 << fls<integer>(x - 1) : 0);
}

template <class integer>
static integer round_down_pow_of_2(integer x)
{
	static_assert(std::is_integral<integer>::value, "param not integral");
	return (x ? 1 << (fls<integer>(x) - 1): 0);
}