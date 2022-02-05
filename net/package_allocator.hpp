#pragma once

#include "struct/lock_free/chunk.h"

namespace net {
class package_allocator
{
public:
	package_allocator(size_t small_size, size_t large_size)
		: samll_packet_pool_(small_size * SMALL_PACKET_SIZE, SMALL_PACKET_SIZE)
		, large_packet_pool_(large_size * LARGE_PACKET_SIZE, LARGE_PACKET_SIZE)
	{}

	void* allocate(size_t size)
	{
		header_t* result = 0;
		if (is_small_packet(size))
		{
			result = static_cast<header_t*>(samll_packet_pool_.pop());
		}
		else if (is_large_packet(size))
		{
			result = static_cast<header_t*>(large_packet_pool_.pop());
		}

		LOG_PROCESS_ERROR_RET(result, 0);

		result->packet_size = size;
#if defined(DEBUG)
		result->magic_code = 0x1234;
#endif
		return result + 1;
	}

	void deallocate(void* ptr)
	{
		header_t* header = static_cast<header_t*>(ptr);
		--header;
#if defined(DEBUG)
		LOG_PROCESS_ERROR(header->magic_code == 0x1234);
		header->magic_code = 0;
#endif
		if (is_small_packet(header->packet_size))
		{
			samll_packet_pool_.push(ptr);
		}
		else if (is_large_packet(header->packet_size))
		{
			large_packet_pool_.push(ptr);
		}
		else
		{
			// ???
			LOG_PROCESS_ERROR(false);
		}
	}

private:
	bool is_small_packet(size_t size) { return size <= SMALL_PACKET_DATA_SIZE; }
	bool is_large_packet(size_t size) { return size <= LARGE_PACKET_DATA_SIZE; }

private:
	struct header_t
	{
		size_t packet_size;
#if defined(DEBUG)
		size_t magic_code;
#endif 
	};

	enum {SMALL_PACKET_DATA_SIZE = 1024 };
	enum {LARGE_PACKET_DATA_SIZE = 1024 * 64};

	enum { SMALL_PACKET_SIZE = SMALL_PACKET_DATA_SIZE + sizeof(header_t) };
	enum { LARGE_PACKET_SIZE = LARGE_PACKET_DATA_SIZE + sizeof(header_t) };

	mtl::lock_free::chunk samll_packet_pool_;
	mtl::lock_free::chunk large_packet_pool_;
};
} // namespace net