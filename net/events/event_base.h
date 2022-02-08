#pragma once

#include "session.h"
namespace net
{
class basic_event
{
public:
	virtual void run() = 0;
};
} //namespace net