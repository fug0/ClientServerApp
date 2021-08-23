#include "../thread_pool/thread_pool.h"

#include "ace/Event_Handler.h"

Thread_Pool::Thread_Pool(void)
    : active_threads_(0)
{
}

int Thread_Pool::open(int pool_size)
{
    return this->activate(THR_NEW_LWP | THR_DETACHED, pool_size);
}

int Thread_Pool::close(u_long flags)
{
    ACE_UNUSED_ARG(flags);

    int counter = active_threads_.value();

    while (counter--)
        this->enqueue(0);

    while (active_threads_.value())
        Sleep(250000);

    return(0);
}

int Thread_Pool::enqueue(ACE_Event_Handler* handler)
{
    void* v_data = (void*)handler;
    char* c_data = (char*)v_data;

    ACE_Message_Block* mb;

    ACE_NEW_RETURN(mb, ACE_Message_Block(c_data), -1);

    if (this->putq(mb) == -1)
    {
        mb->release();
        return -1;
    }

    return 0;

}
class Counter_Guard
{
public:
    Counter_Guard(Thread_Pool::counter_t& counter)
        : counter_(counter)
    {
        ++counter_;
    }

    ~Counter_Guard(void)
    {
        --counter_;
    }

protected:
    Thread_Pool::counter_t& counter_;
};

class Message_Block_Guard
{
public:
    Message_Block_Guard(ACE_Message_Block*& mb)
        : mb_(mb)
    {
    }

    ~Message_Block_Guard(void)
    {
        mb_->release();
    }

protected:
    ACE_Message_Block*& mb_;
};

int Thread_Pool::svc(void)
{
    ACE_Message_Block* mb;

    Counter_Guard counter_guard(active_threads_);

    while (this->getq(mb) != -1)
    {

        Message_Block_Guard message_block_guard(mb);

        char* c_data = mb->base();

        if (c_data)
        {
            void* v_data = (void*)c_data;

            ACE_Event_Handler* handler = (ACE_Event_Handler*)v_data;

            if (handler->handle_input(ACE_INVALID_HANDLE) == -1)
            {
                handler->handle_close(handler->get_handle(), 0);
            }
        }
        else
        {
            return 0;
        }
    }

    return 0;
}