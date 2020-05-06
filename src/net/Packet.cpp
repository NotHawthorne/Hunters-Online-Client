#include "../../includes/packet.h"
#include <stdio.h>

Packet::Packet(void *newdata, size_t newsize, int newtype)
{
	data = newdata;
	size = newsize;
	type = newtype;
}

Packet	&Packet::operator=(Packet &p)
{
	data = p.data;
	size = p.size;
	return (*this);
}

PacketManager::PacketManager()
{
	bzero(_id, 16);
}

PacketManager::~PacketManager()
{
}

PacketManager::PacketManager(char *id)
{
	bzero(_id, 16);
	memcpy(_id, id, strlen(id) > 16 ? 15 : strlen(id));
}

int	PacketManager::sendHeaderPacket(int type, int amt)
{
	t_packet_header	*h = new t_packet_header;
	Packet			*p;

	bzero(h, sizeof(t_packet_header));	
	memcpy(h->id, _id, strlen(_id));
	h->cmd = type;
	h->amt = amt;
	p = new Packet((void*)h, sizeof(t_packet_header), CLIENT_HEADER);
	//q.reserve(q.size() + 1);
	q.push(p);
	return (1);
}

int	PacketManager::sendLoginPacket(char *pass)
{
	t_login_packet	*p = new t_login_packet;

	bzero(p, sizeof(t_login_packet));
	memcpy(p->pass, pass, strlen(pass) > 16 ? 15 : strlen(pass));
	sendHeaderPacket(CLIENT_LOGIN, 1);
	q.push(new Packet((void*)p, sizeof(t_login_packet), CLIENT_LOGIN));
	return (1);
}

int	PacketManager::sendChatPacket(char *dest, char *data)
{
	t_chat_packet	*p = new t_chat_packet;

	bzero(p, sizeof(t_chat_packet));
	memcpy(p->dest, dest, strlen(dest) > 15 ? 15 : strlen(dest));
	memcpy(p->data, data, strlen(data) > 239 ? 239 : strlen(dest));
	sendHeaderPacket(CLIENT_CHAT, 1);
	q.push(new Packet((void*)p, sizeof(t_chat_packet), CLIENT_CHAT));
	return (1);
}

int	PacketManager::sendInvenCmdPacket(int type, int id, char *dest)
{
	t_inven_cmd_packet	*p = new t_inven_cmd_packet;

	bzero(p, sizeof(t_inven_cmd_packet));
	p->cmd = type;
	p->item_id = id;
	if (dest)
		memcpy(p->dest, dest, strlen(dest) > 15 ? 15 : strlen(dest));
	sendHeaderPacket(type, 1);
	q.push(new Packet((void*)p, sizeof(t_inven_cmd_packet), type));
	return (1);
}

int	PacketManager::emptyQueue(int fd)
{
	if (q.size() <= 0)
		return (1);
	while (!q.empty())
	{
		Packet	*p = q.front();
		write(fd, p->data, p->size);
		q.pop();
	}
	usleep(200);
	return (1);
}
