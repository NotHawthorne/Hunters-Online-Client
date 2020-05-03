#ifndef SESSION_H
# define SESSION_H

# include "config.h"
# include "packet.h"
# include <arpa/inet.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <vector>
# include <map>
# include <ctime>

class	SessionManager
{
public:
	int					conn_fd;
	struct sockaddr_in	server_addr;
	PacketManager		*packet_man;
	PacketManager		*packet_in;

	SessionManager();
	~SessionManager();
	int					startLoop();
	int					getCharList(int amt);
};

#endif
