#ifndef PLAYER_H
# define PLAYER_H

# include "packet.h"

class	Character
{
public:
	char	name[16];
	int		pclass;
	int		race;
	int		lvl;
	int		hp;
	int		max_hp;
	int		mana;
	int		max_mana;
	Character(t_character_packet *p);
};

class	Player
{
public:
	Character	*c;
	int			str;
	int			intel;
	int			dex;
	int			exp;
	int			gold;
};

#endif
