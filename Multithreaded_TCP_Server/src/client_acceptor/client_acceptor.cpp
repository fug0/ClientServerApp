#include "../client_acceptor/client_acceptor.h"

Client_Acceptor::Client_Acceptor(int concurrency) : concurrency_(concurrency), the_thread_pool_(private_thread_pool_)
{
}


Client_Acceptor::Client_Acceptor(Thread_Pool& thread_pool) : concurrency_(thread_pool_), the_thread_pool_(thread_pool)
{
}


Client_Acceptor::~Client_Acceptor(void)
{
    if (this->concurrency() == thread_pool_ && thread_pool_is_private())
        thread_pool()->close();
}


int Client_Acceptor::open(const ACE_INET_Addr& addr, ACE_Reactor* reactor, int pool_size)
{
    if (this->concurrency() == thread_pool_ && thread_pool_is_private())
        thread_pool()->open(pool_size);

    return inherited::open(addr, reactor);
}


int Client_Acceptor::close(void)
{
    if (this->concurrency() == thread_pool_ && thread_pool_is_private())
        thread_pool()->close();

    return inherited::close();
}