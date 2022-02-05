#pragma once

#include "operation/operation.h"
#include "socket_types.h"
#include "logger/log.h"

namespace net {
class descriptor_data
{
	typedef std::function<void(void)> functor_t;
public:
	descriptor_data()
	{
		clear_ops();
	}
	
	void set_read_op() { ops |= read; }
	void set_write_op() { ops |= write; }
	void set_except_op() { ops |= except; }
	void clear_ops() { ops = 0; }

	void set_read_callback(functor_t f) { read_handler_ = f; }
	void set_write_callback(functor_t f) { write_handler_ = f; }
	void set_except_callback(functor_t f) { except_handler_ = f; }
	int& descriptor() { return descriptor_; }

	void perform_io()
	{
		if (ops & except)
		{
			if (except_handler_) except_handler_();
		}
		if (ops & write)
		{
			if (write_handler_) write_handler_();
		}
		if (ops & read)
		{
			if (read_handler_) read_handler_();
		}

		clear_ops();
	}
private:
	enum op_types { read = 1, write = 2, except = 4 };

	int ops;
	functor_t read_handler_;
	functor_t write_handler_;
	functor_t except_handler_;
	int descriptor_;
};
}