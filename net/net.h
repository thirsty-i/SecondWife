#pragma once

#include "asio.hpp"
#include <cstdint>
#include <thread>
#include "mtl/struct/lock_free/queue.h"
#include "mtl/pool/object_pool.h"
#include "mtl/design_pattern/singleton.h"
#include "socket_acceptor.h"
#include "socket_session.h"

class net final
    : public mtl::singleton<net>
{
private:
    net();
public:
    using event_t = std::function<void(void)>;
    ~net();
    socket_acceptor_ptr create_acceptor();

    socket_session_ptr create_session();

    void start();

    asio::io_context& get_context() { return io_context_; }

    void push_event(event_t&& event);
    bool pop_event(event_t& event);
    void main_loop();
private:
    asio::io_context io_context_;
    std::unique_ptr<std::thread> thread_;
    mtl::queue<event_t> event_queue_;
    mtl::object_pool<socket_session> session_pool_;
    std::vector<socket_acceptor_ptr> acceptors_;

private:
	friend class mtl::singleton<net>;
};