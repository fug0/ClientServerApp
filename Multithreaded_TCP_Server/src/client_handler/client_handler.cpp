#include "../client_acceptor/client_acceptor.h"
#include "../client_handler/client_handler.h"

#define  REGISTER_MASK ACE_Event_Handler::READ_MASK
#define REMOVE_MASK (ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL)


Client_Handler::Client_Handler(void) : client_acceptor_(0), creator_(ACE_Thread::self()), DATA_SIZE(0)
{
}

Client_Handler::~Client_Handler(void)
{
    this->peer().close();
}

int Client_Handler::concurrency(void)
{
    return this->client_acceptor()->concurrency();
}

Thread_Pool* Client_Handler::thread_pool(void)
{
    return this->client_acceptor()->thread_pool();
}

int Client_Handler::open(void* acceptor)
{
    client_acceptor((Client_Acceptor*)acceptor);

    this->reactor(client_acceptor()->reactor());

    ACE_INET_Addr addr;

    if (this->peer().get_remote_addr(addr) == -1)
        return -1;

    if (this->reactor()->register_handler(this, REGISTER_MASK) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) can't register with reactor\n"), -1);

    ACE_DEBUG((LM_DEBUG, "(%P|%t) connected with %s\n", addr.get_host_name()));

    return 0;
}

void Client_Handler::destroy(void)
{
    this->reactor()->remove_handler(this, REMOVE_MASK);
    delete this;
}

int Client_Handler::close(u_long flags)
{
    ACE_UNUSED_ARG(flags);

    this->destroy();

    return 0;
}

int Client_Handler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    ACE_UNUSED_ARG(handle);
    ACE_UNUSED_ARG(mask);

    delete this;

    return 0;
}

int Client_Handler::handle_input(ACE_HANDLE handle)
{
    ACE_UNUSED_ARG(handle);

    if (concurrency() == Client_Acceptor::thread_pool_)
    {
        if (ACE_OS::thr_equal(ACE_Thread::self(), creator_))
        {
            this->reactor()->remove_handler(this, REMOVE_MASK);
            return this->thread_pool()->enqueue(this);
        }
    }

    int rval;

    DATA_TYPE* data_buf = new DATA_TYPE[DATA_SIZE];

    if (this->DATA_SIZE == 0)
    {
        char buf[BUFSIZ];
        rval = this->process(buf, sizeof(buf));
    }
    else
    {
        rval = this->process(data_buf, DATA_SIZE);
    }

    if (concurrency() == Client_Acceptor::thread_pool_)
    {
        if (rval != -1)
            this->reactor()->register_handler(this, REGISTER_MASK);
    }

    return rval;
}

//

template <typename T>
T Decoder(std::string* encoded_length)
{
    T decoded_length = 0;
    if ((encoded_length->front()) == '0')
    {
        for (int i = 0; i < encoded_length->length(); i++)
        {
            decoded_length <<= 1;
            decoded_length |= (encoded_length->c_str()[i] - '0');
        }
    }
    else if ((encoded_length->front()) == '1')
    {
        std::string temp_str = "";

        for (uint8_t i = 8; i < encoded_length->length(); i++)
        {
            if (encoded_length->c_str()[i] != ' ')
            {
                temp_str += encoded_length->at(i);
            }
        }

        for (uint8_t i = 0; i < temp_str.length(); i++)
        {
            decoded_length <<= 1;
            decoded_length |= (temp_str.c_str()[i] - '0');
        }
    }

    return decoded_length;
}

//

int Client_Handler::process(DATA_TYPE* rdbuf, int rdbuf_len)
{
    ssize_t bytes_read;
    switch ((bytes_read = this->peer().recv(rdbuf, rdbuf_len)))
    {
    case -1:
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p bad read\n", "client"), -1);
    case 0:
        ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) closing daemon (fd = %d)\n", this->get_handle()), -1);
    default:
        rdbuf[bytes_read] = '\x0';

        if (this->DATA_SIZE == 0)
        {
            std::string str(rdbuf);

            this->DATA_SIZE = Decoder<int>(&str) * 8;

            ACE_DEBUG((LM_DEBUG, "(%P|%t) encoded length: %s\n", rdbuf));
        }
        else
        {
            ACE_DEBUG((LM_DEBUG, "(%P|%t) from client: %s\n", rdbuf));
        }
    }
    return 0;
}


