#pragma once

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"

class Client_Acceptor;
class Thread_Pool;

class Client_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
    typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> inherited;

    typedef char DATA_TYPE;

    Client_Handler(void);

    void destroy(void);

    int open(void* acceptor);

    int close(u_long flags = 0);

    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);

    int handle_input(ACE_HANDLE handle);

protected:
    int process(DATA_TYPE* rdbuf, int rdbuf_len);

    ~Client_Handler(void);

    Client_Acceptor* client_acceptor(void)
    {
        return this->client_acceptor_;
    }

    void client_acceptor(Client_Acceptor* _client_acceptor)
    {
        this->client_acceptor_ = _client_acceptor;
    }

    int concurrency(void);

    int DATA_SIZE;

    Thread_Pool* thread_pool(void);

    Client_Acceptor* client_acceptor_;

    ACE_thread_t creator_;
};

#endif