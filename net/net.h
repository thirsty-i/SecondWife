#pragma once

#include "asio.hpp"
#include <cstdint>
#include <thread>
#include "mtl/struct/lock_free/queue.h"

class net final
{
public:
    net();
    ~net();
    void create_acceptor(const char* address, uint32_t port);
    void connect(const char* address, uint32_t port);

    void start();
    void create_session();
    
private:
    asio::io_context io_context_;

    std::unique_ptr<std::thread> thread_;
    //mtl::queue<>
};