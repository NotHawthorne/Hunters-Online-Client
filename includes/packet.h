#ifndef PACKET_H
# define PACKET_H

# include <queue>
# include <string.h>
# include <unistd.h>

enum client_packet_types
{
	CLIENT_LOGIN,
	CLIENT_REG,
	CLIENT_SELECT_CHAR,
	CLIENT_CHAT,
	CLIENT_ITEMS,
	CLIENT_PLAYERS,
	CLIENT_EQUIP,
	CLIENT_DROP,
	CLIENT_GIVE,
	CLIENT_LIST,
	CLIENT_SKILLS,
	CLIENT_HEADER
};

enum server_packet_types
{
	SERVER_LOGIN_FAIL,
	SERVER_LOGIN_SUCCESS,
	SERVER_NOTIFY,
	SERVER_CHAT,
	SERVER_STATUS,
	SERVER_CHARACTERS,
	SERVER_CHARACTER
};

typedef struct	s_packet_header
{
	char		id[16];
	int			cmd;
	int			amt;
}				t_packet_header;

typedef struct	s_login_packet
{
	char		pass[16];
}				t_login_packet;

typedef struct	s_chat_packet
{
	char		dest[16];
	char		data[240];
}				t_chat_packet;

typedef struct	s_inven_cmd_packet
{
	int			item_id;
	int			cmd;
	char		dest[16];
}				t_inven_cmd_packet;

typedef struct	s_status_packet
{
	int	g;
	int	h;
	int	str;
	int	intel;
	int	dex;
	int	hp;
	int	max_hp;
	int	mana;
	int	max_mana;
	int	groupid;
	int	lvl;
	int	exp;
	int	gold_exponent;
}				t_status_packet;

typedef struct	s_character_packet
{
	char	name[16];
	int		pclass;
	int		race;
	int		level;
	int		hp;
	int		max_hp;
	int		mana;
	int		max_mana;
}				t_character_packet;

class	Packet
{
public:
	void	*data;
	size_t	size;
	int		type;
	Packet(void *newdata, size_t newsize, int newtype);
	Packet&	operator=(Packet &p);
};

class	PacketManager
{
public:
	char					_id[16];
	char					_pass[16];
	PacketManager(char *id);
	PacketManager();
	~PacketManager();

	std::queue<Packet *>	q;

	int		sendHeaderPacket(int type, int amt);
	int		sendLoginPacket(char *pass);
	int		sendChatPacket(char *dest, char *data);
	int		sendInvenCmdPacket(int type, int id, char *dest);
	int		emptyQueue(int fd);
};

#endif
