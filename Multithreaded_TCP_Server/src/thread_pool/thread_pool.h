#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "ace/Task.h"
#include "ace/Atomic_Op_T.h"

class ACE_Event_Handler;

class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>
{
public:
    typedef ACE_Task<ACE_MT_SYNCH> inherited;

    enum size_t
    {
        default_pool_size_ = 5
    };

    Thread_Pool(void);

    int open(int pool_size = default_pool_size_);

    virtual int open(void* void_data)
    {
        return inherited::open(void_data);
    }

    virtual int close(u_long flags = 0);

    int enqueue(ACE_Event_Handler* handler);

    typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> counter_t;

protected:

    int svc(void);

    counter_t active_threads_;
};

#endif 