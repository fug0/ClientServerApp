#pragma once

#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"

#include "../client_handler/client_handler.h"
#include "../thread_pool/thread_pool.h"

typedef ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR> Client_Acceptor_Base;

class Client_Acceptor : public Client_Acceptor_Base
{
public:
    typedef Client_Acceptor_Base inherited;

    enum concurrency_t
    {
        single_threaded_,
        thread_pool_
    };

    Client_Acceptor(int concurrency = thread_pool_);

    Client_Acceptor(Thread_Pool& thread_pool);

    ~Client_Acceptor(void);

    int open(const ACE_INET_Addr& addr, ACE_Reactor* reactor, int pool_size = Thread_Pool::default_pool_size_);

    int close(void);

    int concurrency(void)
    {
        return this->concurrency_;
    }

    Thread_Pool* thread_pool(void)
    {
        return &this->the_thread_pool_;
    }

    int thread_pool_is_private(void)
    {
        return &the_thread_pool_ == &private_thread_pool_;
    }

protected:
    int concurrency_;

    Thread_Pool private_thread_pool_;

    Thread_Pool& the_thread_pool_;
};

#endif