#include "net.h"

net::net()
    : io_context_(1)
{

}

net::~net()
{
    if(thread_->joinable())
        thread_->join();
}


void net::create_acceptor(const char* address, uint32_t port)
{
    
}


void net::connect(const char* address, uint32_t port)
{

}

void net::start()
{
    thread_.reset(new std::thread([this]() { 
        io_context_.run();
    }));
}