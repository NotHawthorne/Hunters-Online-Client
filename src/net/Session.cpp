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
	int	f = fcntl(conn_fd, F_GETFL);
	fcntl(conn_fd, F_SETFL, f | O_NONBLOCK);
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
		t_packet_header	*h = new t_packet_header;
		bzero(h, sizeof(t_packet_header));
		packet_man->emptyQueue(conn_fd);
		if ((read_bytes = recv(conn_fd, h, sizeof(t_packet_header), MSG_WAITALL) > 0))
		{
			printf("%d %d\n", (int)sizeof(t_packet_header), read_bytes);
			/*
			while (read_bytes < (int)sizeof(t_packet_header))
			{
				printf("here\n");
				int rb;
				if ((rb = read(conn_fd, ((char*)h) + read_bytes,
						sizeof(t_packet_header) - read_bytes)) > 0)
				{
					read_bytes += rb;
					printf("%d\n", rb);
				}
			}*/
			printf("recieved data %d amt %d %s %d\n", h->cmd, h->amt, h->id, read_bytes);
			switch (h->cmd)
			{
				case SERVER_LOGIN_FAIL:
					return (-1);
					break ;
				case SERVER_LOGIN_SUCCESS:
					getCharList(h->amt);
					break ;
			}
		}
		else
			packet_man->emptyQueue(conn_fd);
		delete h;
	}
	return (0);
}
