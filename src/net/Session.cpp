#include "../../includes/session.h"

SessionManager::SessionManager()
{
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	if (!(inet_pton(AF_INET, ADDRESS, &server_addr.sin_addr)))
		return ;
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (conn_fd <= 0 || connect(conn_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("CANNOT CONNECT\n");
		return ;
	}
	fcntl(conn_fd, F_SETFL, fcntl(conn_fd, F_GETFL) | O_NONBLOCK);
}

SessionManager::~SessionManager()
{
	close(conn_fd);
}

int	SessionManager::getCharList(int amt)
{
	for (int i = 0; i != amt; i++)
	{
		int read_bytes = 0;
		t_character_packet	*p = new t_character_packet;
		while (read_bytes < (int)sizeof(t_character_packet))
		{
			int rb = read(conn_fd, ((char*)p) + read_bytes, sizeof(t_character_packet) - read_bytes);
			if (rb > 0)
				read_bytes += rb;
		}
		packet_in->q.push(new Packet((void*)p, sizeof(t_character_packet), SERVER_CHARACTER));
	}
	printf("GOT THINGS\n");
	return (amt);
}

int	SessionManager::startLoop()
{
	int		read_bytes = 0;

	
	while (1)
	{
		read_bytes = 0;
		t_packet_header	h;
		if ((read_bytes = read(conn_fd, &h, sizeof(t_packet_header)) > 0))
		{
			while (read_bytes < (int)sizeof(t_packet_header))
			{
				int rb;
				if ((rb = read(conn_fd, ((char*)&h) + read_bytes,
						sizeof(t_packet_header) - read_bytes)) > 0)
					read_bytes += rb;
			}
			printf("recieved data %d\n", h.cmd);
			switch (h.cmd)
			{
				case SERVER_LOGIN_FAIL:
					return (-1);
					break ;
				case SERVER_LOGIN_SUCCESS:
					getCharList(h.amt);
					break ;
			}
		}
		else
			packet_man->emptyQueue(conn_fd);
	}
	return (0);
}
