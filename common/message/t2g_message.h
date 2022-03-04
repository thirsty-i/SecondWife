#pragma once

#include "msg_def.h"

enum class t2g_message_id
{
	message_begin = t2g_message_begin,

	t2g_test_net,

	message_total,

};

#pragma pack(push, 1)
struct t2g_test_net : msg_header
{
	int32_t magic_code;
};

#pragma pack(pop)

static_assert((size_t)t2g_message_id::message_total <= (size_t)t2g_message_end, "message_total <= t2g_message_end");
