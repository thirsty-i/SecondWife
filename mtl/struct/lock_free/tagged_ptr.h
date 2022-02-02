#pragma once
#include <cstdint>

namespace mtl {
namespace lock_free {
	template <class T>
	class tagged_ptr
	{
	public:
		using pointer = T*;
		using tag_type = uint16_t;
		using compressed_ptr_t = uint64_t;

		union cast_unit
		{
			compressed_ptr_t ptr;
			struct 
			{
				unsigned reserved_1 : 32;
				unsigned reserved_2 : 16;
				unsigned tag : 16;
			};
		};

		static const compressed_ptr_t ptr_mask = 0xffffffffffffUL; //(1L<<48L)-1;

		static_assert(sizeof(void*) == 8, "tagged_ptr_t is not equal in size to original pointer");
		static_assert(sizeof(cast_unit) == 8, "cast_unit size error");
	
		explicit tagged_ptr(pointer ptr, tag_type tag = 0)
			: cu_(_make(ptr, tag)) {};

		tagged_ptr& operator= (tagged_ptr const& p) = default;

		pointer get_ptr() 
		{ 
			return reinterpret_cast<pointer>(cu_.ptr & ptr_mask); 
		}

		tag_type get_tag() 
		{
			return cu_.tag; 
		}

		void set_ptr(pointer ptr) 
		{ 
			set(ptr, get_tag());
		}

		void set_tag(tag_type tag) 
		{ 
			cu_.tag = tag; 
		}
		
		void set(pointer ptr, tag_type tag) 
		{ 
			cu_ = _make(ptr, tag); 
		}
	private:
		cast_unit _make(pointer ptr, tag_type tag)
		{
			cast_unit cu;
			cu.ptr = reinterpret_cast<compressed_ptr_t>(ptr);
			cu.tag = tag;
			return cu;
		}
	private:
		cast_unit cu_;
	};
}
}