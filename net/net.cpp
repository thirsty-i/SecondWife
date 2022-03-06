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
    //return session_pool_.create(io_context_);
    return std::shared_ptr<socket_session>(new socket_session(io_context_));
}

void net::start()
{
    thread_.reset(new std::thread([this]() { 
        io_context_.run();
    }));
}

void net::push_event(event_t&& event)
{
    event_queue_.push(std::move(event));
}

bool net::pop_event(event_t& event)
{
    return event_queue_.pop(event);
}

void net::main_loop()
{
    event_t event;
    while (pop_event(event))
    {
        event.operator()();
    }
}