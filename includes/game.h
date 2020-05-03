#ifndef GAME_H
# define GAME_H

# include <sqlite3.h>
# include <map>
# include "display.h"
# include "item.h"
# include "packet.h"
# include "effect.h"
# include "player.h"

//class Auction;

class	GameManager
{
public:
	sqlite3						*db;
	Player						*p;
	Character					*cur_character;
	PacketManager				*packet_in;
	DisplayManager				*display;

	std::map<int, Character *>	characters;
	std::map<int, Item *>		inventory;
	std::map<int, Item *>		equipment;
	std::map<int, ItemBase *>	item_base;
	std::map<int, Aura *>		auras;
	std::map<int, Item *>		container;
	//std::map<int, Auction *>	auctions;

	GameManager();
	~GameManager();
	int							emptyQueue();
	int							getCharList(t_packet_header *h);
};

#endif
