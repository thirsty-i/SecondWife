#pragma once

using message_size_t = uint16_t;

struct msg_base
{
	message_size_t length;
};

struct msg_header : public msg_base
{
	uint32_t msg_id;
};

enum { MESSAGE_LENGTH_SIZE = sizeof(message_size_t) };
enum { MSG_HEADER_SIZE = sizeof(msg_header) };
enum { MAX_MESSAGE_SIZE = std::numeric_limits<uint16_t>::max()};


enum
{
	t2g_message_begin = 0,
	t2g_message_end = 1024,

	g2t_message_begin,
	g2t_message_end = 2048,

	c2g_message_begin,
	c2g_message_end = 4096,

	g2c_message_bein,
	g2c_message_end = 8192,
};