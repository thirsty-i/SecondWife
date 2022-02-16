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
    ~net();
    // asio::ip::tcp::acceptor* create_acceptor(const char* address, uint32_t port);
    // asio::ip::tcp::socket* connect(const char* address, uint32_t port);
    socket_acceptor::pointer create_acceptor();
    socket_session::pointer  create_session();

    void start();

    asio::io_context& get_context() { return io_context_; }
    
private:
    asio::io_context io_context_;
    std::unique_ptr<std::thread> thread_;
    //mtl::queue<>

    friend class mtl::singleton<net>;

    mtl::object_pool<socket_session> session_pool_;
    std::vector<socket_acceptor::pointer> acceptors_;
};