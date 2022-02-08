#pragma once

struct msg_header_t
{
	size_t msg_size;
	char data[0];
};

enum { MAX_MSG_SIZE = 64 * 1024}; // 2^16