#include "net.h"

net::net()
    : io_context_(1)
    , session_pool_(1024)
{

}

net::~net()
{
    if(thread_ && thread_->joinable())
        thread_->join();
    
}

socket_acceptor_ptr net::create_acceptor()
{
    acceptors_.push_back(std::make_shared<socket_acceptor>(io_context_));
    return acceptors_.back();
}

socket_session_ptr net::create_session()
{
    return session_pool_.create();
}

// asio::ip::tcp::socket* net::connect(const char* address, uint32_t port)
// {

// }

void net::start()
{
    thread_.reset(new std::thread([this]() { 
        io_context_.run();
    }));
}