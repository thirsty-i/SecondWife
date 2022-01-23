#pragma once

#include <cstdint>

namespace mtl {
namespace lock_free {
	template <class T>
	class tagged_ptr
	{
		using value_type = T;
		using pointer = T*;
		using tag_type = uint16_t;

		struct cast_unit
		{
			pointer ptr  : 48;
			uint16_t tag;
		};

		static_assert<sizeof(void*) == sizeof(cast_unit), "tagged_ptr_t is not equal in size to origer pointer">;
		static_assert<sizeof(cast_unit) == 8, "cast_unit is not 64 bit">;
	public:
		explicit tagged_ptr(pointer ptr, tag_type tag = 0)
			: cu_(_make(ptr, tag)) {};

		tagged_ptr& operator= (tagged_ptr const& p) = default;

		pointer get_ptr() { return cu_.ptr; }
		uint16_t get_tag() { return cu_.tag; }

		void set_ptr(pointer ptr) { cu_.ptr = ptr; }
		void set_tag(tag_type tag) { cu_.tag = tag; }
		void set(pointer ptr, tag_type tag) { cu_ = _make(ptr, tag); }
	private:
		cast_unit _make(pointer ptr, tag_type tag)
		{
			cast_unit cu;
			cu.ptr = ptr;
			cu.tag = tag;
			return cu;
		}
	private:
		cast_unit cu_;
	};
}
}