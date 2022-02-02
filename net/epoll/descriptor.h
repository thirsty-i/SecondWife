#pragma once

class descriptor
{
	class epoll;

	epoll* epoll_;
	int descriptor_;
};