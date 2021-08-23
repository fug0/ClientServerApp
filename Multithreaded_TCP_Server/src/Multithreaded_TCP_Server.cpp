// Multithreaded_TCP_Server.cpp : Defines the entry point for the application.
//

#include "Multithreaded_TCP_Server.h"

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int main(int argc, char* argv[])
{
	ACE_UNUSED_ARG(argc);
	ACE_UNUSED_ARG(argv);

	ACE_Reactor reactor;

	Client_Acceptor peer_acceptor;

	if (peer_acceptor.open(ACE_INET_Addr(PORT), &reactor) == -1)
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "open"), -1);

	ACE_DEBUG((LM_DEBUG, "(%P|%t) starting up a server\n"));

	while (true)
		reactor.handle_events();

	ACE_DEBUG((LM_DEBUG, "(%P|%t) shutting down a server\n"));

	return 0;
}
