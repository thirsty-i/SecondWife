#pragma once

#include "msg_def.h"

enum class g2t_message_id
{
	message_begin = g2t_message_begin,

	g2c_test_net,

	message_total,

};
static_assert((size_t)g2t_message_id::message_total <= (size_t)g2t_message_end, "message_total <= g2t_message_end");

#pragma pack(push, 1)
struct g2c_test_net : msg_header
{
	int32_t magic_code;
};

#pragma pack(pop)


