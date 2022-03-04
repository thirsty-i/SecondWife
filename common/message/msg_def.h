#pragma once

struct msg_base
{
	size_t length;
};

struct msg_header : public msg_base
{
	size_t msg_id;
};

enum
{
	t2g_message_begin = 0,
	t2g_message_end = 1024,

	c2g_message_begin,
	c2g_message_end = 2048,

	g2t_message_begin,
	g2t_message_end = 4096,

	g2c_message_bein,
	g2c_message_end = 8192,
};