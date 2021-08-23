// Client.cpp : Defines the entry point for the application.
//

#include "Client.h"


class Client : public ACE_SOCK_Stream
{
public:
    Client(void);

    Client(const char* server, u_short port);

    int open(const char* server, u_short port);

    int initialized(void) { return initialized_; }
    int error(void) { return error_; }

    Client& operator<< (ACE_SString& str);
    Client& operator<< (const char* str);
    Client& operator<< (int n);

protected:
    u_char initialized_;
    u_char error_;
};

Client::Client(void)
{
    initialized_ = 0;
    error_ = 0;
}


Client::Client(const char* server, u_short port)
{
    initialized_ = 0;
    error_ = 0;
    this->open(server, port);
}

int Client::open(const char* server, u_short port)
{
    ACE_SOCK_Connector connector;
    ACE_INET_Addr addr(port, server);

    if (connector.connect(*this, addr) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "%p\n", "open"), -1);
    initialized_ = 1;
    return 0;
}

Client& Client::operator<< (ACE_SString& str)
{
    if (initialized() && !error())
    {
        const char* cp = str.fast_rep();

        if (this->send_n(cp, ACE_OS::strlen(cp)) == -1)
            error_ = 1;
    }
    else
        error_ = 1;

    return *this;
}

Client& Client::operator<< (const char* str)
{
    ACE_SString newStr(str);

    *this << newStr;

    return *this;

}

Client& Client::operator<< (int n)
{
    char buf[BUFSIZ];

    sprintf(buf, "(%d)\n", n);

    ACE_SString newStr(buf);

    *this << newStr;

    return *this;
}

//

template <typename T>
void Encoder(T* length, std::string*& encoded_length)
{
    if (((*length) > 0) && ((*length) < 128))
    {
        (*encoded_length) += "0";
        for (int8_t i = 6; i >= 0; i--)
        {
            (*encoded_length) += std::to_string((((*length) >> i) & 1));
        }
    }
    else if ((*length) > 127)
    {
        (*encoded_length) += "1";
        uint8_t octet_number = 1;
        int bit_number = 8;

        bool isLengthEnd = false;
        while (isLengthEnd == false)
        {
            for (int i = 0; i < 8; i++)
            {
                if (((*length) >> (bit_number + i)) & 1)
                {
                    octet_number++;
                    bit_number += 8;
                    break;
                }
                else if (i == 7)
                {
                    isLengthEnd = true;
                }
            }
        }

        for (int8_t i = 6; i >= 0; i--)
        {
            (*encoded_length) += std::to_string(((octet_number >> i) & 1));
        }

        for (int i = bit_number - 1; i >= 0; i--)
        {
            if (((i + 1) % 8) == 0)
            {
                (*encoded_length) += " ";
            }
            (*encoded_length) += std::to_string((((*length) >> i) & 1));
        }

    }
    else
    {
        ACE_DEBUG((LM_DEBUG, "Incorrect length value\n"));
    }
}

//

int main(int argc, char* argv[])
{
    const char* server_host = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_HOST;
    u_short     server_port = argc > 2 ? ACE_OS::atoi(argv[2]) : ACE_DEFAULT_SERVER_PORT;
    int         max_iterations = argc > 3 ? ACE_OS::atoi(argv[3]) : 4;

    Client peer;

    if (peer.open(server_host, server_port) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "%p\n", "open"), -1);

    ACE_DEBUG((LM_DEBUG, "(%P|%t) connected to server, type \"disconnect\" to disconnect from server\n"));

    //
    typedef int LENGTH_TYPE;

    LENGTH_TYPE length = 0;
    LENGTH_TYPE* l_pointer = &length;

    std::string message = "";

    while (message != "disconnect")
    {
        if (length == 0)
        {
            ACE_DEBUG((LM_DEBUG, "(%P|%t) Input length: \n"));

            std::string encoded_length = "";
            std::string* el_pointer = &encoded_length;

            std::cin >> length;

            Encoder(l_pointer, el_pointer);

            peer << encoded_length.c_str();
        }
        else
        {
            std::cin.ignore(1, '\n');
            std::getline(std::cin, message);
            peer << message.c_str();
        }


        if (peer.error())
            ACE_ERROR_RETURN((LM_ERROR, "%p\n", "send"), -1);
        else
            Sleep(100);
    }
    //

    if (peer.close() == -1)
        ACE_ERROR_RETURN((LM_ERROR, "%p\n", "close"), -1);

    return 0;
}

