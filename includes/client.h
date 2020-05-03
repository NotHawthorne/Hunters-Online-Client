#ifndef CLIENT_H
# define CLIENT_H

#include "session.h"
#include "packet.h"
#include "display.h"
#include "game.h"

class	Client
{
public:
	SessionManager	*session_man;
	PacketManager	*packet_man;
	PacketManager	*packet_in;
	DisplayManager	display_man;
	GameManager		game_man;
};

#endif
