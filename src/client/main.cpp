#include "../../includes/display.h"
#include "../../includes/client.h"
#include <pthread.h>

void	*listener(void *ptr)
{
	Client	*cli = (Client*)ptr;
	cli->session_man->startLoop();
	return (NULL);
}

int		main(void)
{
	Client			*cli = new Client();
	int				to_exit = 0;
	pthread_t		listen_thread;
	char			*a;
	char			*b;

	cli->session_man = new SessionManager();
	while (!to_exit)
	{
	
		switch (((LoginMenu*)(cli->display_man.curmenu))->handleInput(cli->display_man.getCh()))
		{
			case 0:
				to_exit = 1;
				break ;
			case 2:
				a = (char*)((LoginMenu*)(cli->display_man.curmenu))->user_str.c_str();
				b = (char*)((LoginMenu*)(cli->display_man.curmenu))->pass_str.c_str();
				cli->packet_man = new PacketManager(a);
				cli->packet_in = new PacketManager(a);
				memcpy(cli->packet_in->_pass, b, strlen(b));
				memcpy(cli->packet_man->_pass, b, strlen(b));
				cli->session_man->packet_man = cli->packet_man;
				cli->session_man->packet_in = cli->packet_in;
				pthread_create(&listen_thread, NULL, listener, cli);
				cli->packet_man->sendLoginPacket(b);
				break ;
		}
		((LoginMenu*)(cli->display_man.curmenu))->update();
	}
	delete cli;
}
